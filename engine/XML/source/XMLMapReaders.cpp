#include <boost/foreach.hpp>
#include "CreatureFactory.hpp"
#include "Game.hpp"
#include "MapUtils.hpp"
#include "TileGenerator.hpp"
#include "XMLMapReaders.hpp"
#include "XMLDataStructures.hpp"

using namespace std;

// XMLMapReader reads in the details from a CustomMap element, and
// creates a MapPtr based on it.
MapPtr XMLMapReader::get_custom_map(const XMLNode& custom_map_node)
{
  MapPtr custom_map;
 
  if (!custom_map_node.is_null())
  {
    XMLNode dimensions_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "Dimensions");
    XMLNode tiles_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "Tiles");
    XMLNode player_start_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "PlayerStart");
    XMLNode initial_placements_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "InitialPlacements");

    string map_id = XMLUtils::get_attribute_value(custom_map_node, "id");
    MapType map_type = static_cast<MapType>(XMLUtils::get_child_node_int_value(custom_map_node, "MapType"));

    Dimensions dim = parse_dimensions(dimensions_node);
    TilesMap tiles = parse_tiles(tiles_node, dim.get_y(), dim.get_x());
    Coordinate player_start_location = parse_coordinate(player_start_node);

    custom_map = MapPtr(new Map(dim));
    
    custom_map->set_map_id(map_id);
    custom_map->set_map_type(map_type);
    custom_map->set_tiles(tiles);
    custom_map->set_permanent(true); // custom maps are always permanent.
    custom_map->add_or_update_location(WorldMapLocationTextKeys::CURRENT_PLAYER_LOCATION, player_start_location);

    parse_initial_placements(initial_placements_node, custom_map);

    // Generate the list of creatures on the map, so that it can be accessed
    // later on.
    custom_map->create_creatures();
  }

  return custom_map;
}

// Parse the rows and columns from the Dimensions element into an actual
// Dimensions engine object.
Dimensions XMLMapReader::parse_dimensions(const XMLNode& dimensions_node)
{
  Dimensions dim;

  if (!dimensions_node.is_null())
  {
    int rows = XMLUtils::get_child_node_int_value(dimensions_node, "Rows", 1);
    int cols = XMLUtils::get_child_node_int_value(dimensions_node, "Columns", 1);

    dim.set_y(rows);
    dim.set_x(cols);
  }

  return dim;
}

// Parse the tiles into a proper tile map that would be used in-game,
// using the provided string.
TilesMap XMLMapReader::parse_tiles(const XMLNode& tiles_node, const int rows, const int cols)
{
  TilesMap tiles;

  int cur_row = 0;
  int cur_col = 0;

  if (!tiles_node.is_null())
  {
    string tile_str = XMLUtils::get_node_value(tiles_node);
    size_t tiles_size = tile_str.size();

    XMLTileMapper tile_mapper;

    if (!tile_str.empty())
    {
      for (string::const_iterator s_it = tile_str.begin(); s_it != tile_str.end(); s_it++)
      {
        TilePtr tile = tile_mapper.create_tile(*s_it);

        if (tile)
        {
          TileKey tile_key = Map::make_map_key(cur_row, cur_col);
          tiles.insert(make_pair(tile_key, tile));

          if (++cur_col >= cols)
          {
            cur_col = 0;
            cur_row++;
          }
        }
      }
    }
  }

  return tiles;
}

// Maps the XML tile (as a character) into a new TilePtr.
TilePtr XMLTileMapper::create_tile(const char xml_tile)
{
  TilePtr tile;

  if (xml_tile == '.')
  {
    tile = TileGenerator::generate(TILE_TYPE_DUNGEON);
  }
  else if (xml_tile == '<')
  {
    tile = TileGenerator::generate_staircase(STAIRCASE_UP);
  }
  else if (xml_tile == '#')
  {
    tile = TileGenerator::generate(TILE_TYPE_ROCK);
  }
  else if (xml_tile == 'T')
  {
    tile = TileGenerator::generate(TILE_TYPE_TREE);
  }
  else if (xml_tile == ',')
  {
    tile = TileGenerator::generate(TILE_TYPE_FIELD);
  }
  else if (xml_tile == '*')
  {
    tile = TileGenerator::generate(TILE_TYPE_BUSH);
  }
  else if (xml_tile == '~')
  {
    tile = TileGenerator::generate(TILE_TYPE_RIVER);
  }

  return tile;
}

// Given a node of type Coord in the schema, return an engine Coordinate.
Coordinate XMLMapReader::parse_coordinate(const XMLNode& coord_node)
{
  Coordinate c(0,0);

  if (!coord_node.is_null())
  {
    c.first = XMLUtils::get_child_node_int_value(coord_node, "Row");
    c.second = XMLUtils::get_child_node_int_value(coord_node, "Col");
  }

  return c;
}

// Parse the initial placement of items and creatures.
void XMLMapReader::parse_initial_placements(const XMLNode& initial_placements_node, MapPtr map)
{
  if (!initial_placements_node.is_null())
  {
    XMLNode creatures_node = XMLUtils::get_next_element_by_local_name(initial_placements_node, "Creatures");
    parse_initial_creature_placements(creatures_node, map);
  }
}

// Parse the initial placement of creatures, and place them on the map at the specified location.
void XMLMapReader::parse_initial_creature_placements(const XMLNode& creatures_node, MapPtr map)
{
  if (!creatures_node.is_null())
  {
    vector<XMLNode> placement_nodes = XMLUtils::get_elements_by_local_name(creatures_node, "Placement");

    BOOST_FOREACH(const XMLNode& placement_node, placement_nodes)
    {
      XMLNode coord_node = XMLUtils::get_next_element_by_local_name(placement_node, "Coord");

      string id = XMLUtils::get_child_node_value(placement_node, "ID");
      Coordinate coord = parse_coordinate(coord_node);

      // Place the specified creature on the map.
      Game* game = Game::instance();

      if (game)
      {
        CreaturePtr creature = CreatureFactory::create_by_creature_id(game->get_action_manager_ref(), id);

        TilePtr placement_tile = map->at(coord);

        if (placement_tile)
        {
          if (MapUtils::is_tile_available_for_creature(placement_tile))
          {
            MapUtils::add_or_update_location(map, creature, coord);
          }
        }
      }
    }
  }
}