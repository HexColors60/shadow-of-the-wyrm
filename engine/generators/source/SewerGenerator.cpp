#include "SewerGenerator.hpp"
#include "CoordUtils.hpp"
#include "Log.hpp"
#include "MapProperties.hpp"
#include "MapUtils.hpp"
#include "RNG.hpp"

using namespace std;

const int SewerGenerator::MIN_WIDTH = 5;
const int SewerGenerator::MAX_WIDTH = 27;
const int SewerGenerator::MIN_HEIGHT = 3;
const int SewerGenerator::MAX_HEIGHT = 7;
const int SewerGenerator::Y_INCR = 2;
const int SewerGenerator::X_INCR = 2;

SewerGenerator::SewerGenerator(const std::string& new_map_exit_id)
: Generator(new_map_exit_id, TileType::TILE_TYPE_SCRUB)
{
}

MapType SewerGenerator::get_map_type() const
{
  return MapType::MAP_TYPE_UNDERWORLD;
}

MapPtr SewerGenerator::generate(const Dimensions& dimensions)
{
  MapPtr result_map = std::make_shared<Map>(dimensions);

  fill(result_map, TileType::TILE_TYPE_ROCK);

  // Generate the sewer sections, and then connect them together.
  generate_sewer_sections(result_map);
  connect_sewer_sections(result_map);

  // Place up and potentially down staircases as appropriate.
  place_staircases(result_map);

  return result_map;
}

void SewerGenerator::generate_sewer_sections(MapPtr result_map)
{
  Dimensions dimensions = result_map->size();

  int rows = dimensions.get_y();
  int cols = dimensions.get_x();

  int length = 0;

  int cur_row = 1;

  // For each row:
  while (cur_row < rows - 1)
  {
    int start_x = RNG::range(1, 5);
    int length = RNG::range(MIN_WIDTH, MAX_WIDTH);

    while (start_x + length < cols - 1)
    {
      // Generate a sewer piece, and place it on the map
      for (int i = start_x; i <= start_x + length; i++)
      {
        TilePtr tile = tg.generate(TileType::TILE_TYPE_SEWER);
        result_map->insert(cur_row, i, tile);
      }

      // Track it.
      Coordinate start(cur_row, start_x);
      Coordinate end(cur_row, start_x + length);

      // Ensure that we're always adding sections with actual length
      if (length > 0)
      {
        sections[cur_row].push_back(make_pair(start, end));
      }

      start_x += (length + X_INCR);
      length = RNG::range(MIN_WIDTH, MAX_WIDTH);
    }

    cur_row += Y_INCR;
  }
}

void SewerGenerator::connect_sewer_sections(MapPtr result_map)
{
  // Now we've got a bunch of horizontal sections, and need to connect them.
  // Iterate through each row in the sections map.  For each section, if
  // it overlaps any sections in the row below, we need a connection,
  // somewhere along the intersecting coordinates.
  for (const auto& row_pair : sections)
  {
    int row_num = row_pair.first;
    vector<pair<Coordinate, Coordinate>> pipe_sections = row_pair.second;

    int next_row = row_num += Y_INCR;
    auto r_it = sections.find(next_row);

    // If we're on the last row, we can stop, as there'll be nothing below.
    if (r_it == sections.end())
    {
      break;
    }
    else
    {
      for (const auto& ps : pipe_sections)
      {
        // Have we stopped overlapping?
        vector<pair<Coordinate, Coordinate>> lower_coords = r_it->second;
        int connections = 0;

        for (const auto& lc : lower_coords)
        {
          if (CoordUtils::starts_after(lc, ps))
          {
            break;
          }
          // Any overlap?
          else
          {
            pair<bool, vector<Coordinate>> overlap = CoordUtils::are_segments_joinable(ps, lc);

            // There's some overlap - take a random coordinate and dig there.
            if (overlap.first == true && !overlap.second.empty())
            {
              connect_section(result_map, overlap);
            }
          }
        }
      }
    }
  }
}

void SewerGenerator::connect_section(MapPtr result_map, const pair<bool, vector<Coordinate>>& overlap)
{
  vector<Coordinate> overlap_coords = overlap.second;
  Coordinate dig_coord = overlap_coords.at(RNG::range(0, overlap_coords.size() - 1));

  if (!MapUtils::adjacent_tiles_match_type(result_map, dig_coord, { Direction::DIRECTION_NORTH, Direction::DIRECTION_SOUTH }, TileType::TILE_TYPE_SEWER))
  {
    Log::instance().error("Bad sewer connection!");
  }

  TilePtr connector = tg.generate(TileType::TILE_TYPE_SEWER);
  result_map->insert(dig_coord, connector);
}

void SewerGenerator::place_staircases(MapPtr result_map)
{
  string depth_increment = get_additional_property(TileProperties::TILE_PROPERTY_DEPTH_INCREMENT);
  bool place_player_on_down_staircase = (depth_increment.empty());

  map<int, vector<pair<Coordinate, Coordinate>>> sections_copy = sections;
  pair<Coordinate, Coordinate> sec = retrieve_and_remove_random_section(sections_copy);
  vector<Coordinate> sec_coords = CoordUtils::get_coordinates_in_range(sec.first, sec.second);

  if (!sec_coords.empty())
  {
    Coordinate up_c = sec_coords.at(RNG::range(0, sec_coords.size()-1));
    place_staircase(result_map, up_c.first, up_c.second, TileType::TILE_TYPE_UP_STAIRCASE, TileType::TILE_TYPE_SEWER_COMPLEX, Direction::DIRECTION_UP, get_permanence(), !place_player_on_down_staircase);
  }
  else
  {
    Log::instance().error("Could not generate up staircase for sewer!");
  }

  sec = retrieve_and_remove_random_section(sections_copy);
  sec_coords = CoordUtils::get_coordinates_in_range(sec.first, sec.second);
  Depth depth = result_map->size().depth();
  string max_depth_property = get_additional_property(UnderworldProperties::UNDERWORLD_STRUCTURE_MAX_DEPTH);

  update_depth_details(result_map);

  if (depth.get_current() < depth.get_maximum() && !sec_coords.empty())
  {
    Coordinate down_c = sec_coords.at(RNG::range(0, sec_coords.size() - 1));
    TilePtr down_tile = result_map->at(down_c.first, down_c.second);
    down_tile->set_additional_property(TileProperties::TILE_PROPERTY_ORIGINAL_MAP_ID, get_additional_property(TileProperties::TILE_PROPERTY_ORIGINAL_MAP_ID));
    down_tile->set_additional_property(UnderworldProperties::UNDERWORLD_STRUCTURE_MAX_DEPTH, max_depth_property);
  }
  else
  {
    Log::instance().error("Could not generate down staircase for sewer!");
  }
}

pair<Coordinate, Coordinate> SewerGenerator::retrieve_and_remove_random_section(map<int, vector<pair<Coordinate, Coordinate>>>& sections_copy)
{
  pair<Coordinate, Coordinate> section;

  auto s_it = sections_copy.begin();
  std::advance(s_it, RNG::range(0, sections_copy.size()-1));
  int rand_key = s_it->first;
  int rand_sec_idx = RNG::range(0, s_it->second.size() - 1);

  // Get the random section
  section = s_it->second.at(rand_sec_idx);

  // Now erase that section from our copy of the map, so that if there is a 
  // down staircase it will have to go in a different section.
  s_it->second.erase(s_it->second.begin() + rand_sec_idx);

  return section;
}