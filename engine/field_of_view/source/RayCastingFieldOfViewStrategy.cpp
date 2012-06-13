#include <sstream>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include "RayCastingFieldOfViewStrategy.hpp"
#include "BresenhamLine.hpp"
#include "MapUtils.hpp"

#include "Conversion.hpp"
#include "Log.hpp"

using std::string;
using std::vector;
using boost::make_shared;

RayCastingFieldOfViewStrategy::RayCastingFieldOfViewStrategy(const bool set_view_property)
: FieldOfViewStrategy(set_view_property)
{
}

MapPtr RayCastingFieldOfViewStrategy::calculate(MapPtr view_map, const Coordinate& centre_coord, const int los_length)
{
  MapPtr fov_map = make_shared<Map>(view_map->size());
  BresenhamLine bl;
  
  int row = centre_coord.first;
  int col = centre_coord.second;
  
  // Always add the centre position to the view map - the creature always knows its own location.
  add_point_to_map(centre_coord, view_map, fov_map);

  Coordinate cur_coord;
  vector<Coordinate> line_points;
  
  // Top and bottom rows
  for (int row_cur_x = col - los_length; row_cur_x <= col + los_length; row_cur_x++)
  {
    // Top
    line_points = bl.get_points_in_line(centre_coord.first, centre_coord.second, row - los_length, row_cur_x);
    add_points_to_map_as_appropriate(line_points, view_map, fov_map);
    
    // Bottom
    line_points = bl.get_points_in_line(centre_coord.first, centre_coord.second, row + los_length, row_cur_x);
    add_points_to_map_as_appropriate(line_points, view_map, fov_map);
  }
  
  // Left and right columns
  for (int col_cur_y = row - los_length; col_cur_y <= row + los_length; col_cur_y++)
  {
    // Top
    line_points = bl.get_points_in_line(centre_coord.first, centre_coord.second, col_cur_y, col - los_length);
    add_points_to_map_as_appropriate(line_points, view_map, fov_map);

    // Bottom
    line_points = bl.get_points_in_line(centre_coord.first, centre_coord.second, col_cur_y, col + los_length);
    add_points_to_map_as_appropriate(line_points, view_map, fov_map);
  }
  
  post_process_to_remove_artifacts(centre_coord, view_map, fov_map);
  
  return fov_map;
}

void RayCastingFieldOfViewStrategy::add_points_to_map_as_appropriate(const std::vector<Coordinate>& coords, MapPtr view_map, MapPtr fov_map)
{
  BOOST_FOREACH(Coordinate c, coords)
  {
    TilePtr tile = view_map->at(c);
    
    if (!tile)
    {
      return;
    }
    else
    {
      add_point_to_map(c, view_map, fov_map);

      if (tile->get_is_blocking())
      {
        return;
      }
    }
  }
}

// Credit where credit is due: this is based on jice's "piece of cake visibility determination" page.
void RayCastingFieldOfViewStrategy::post_process_to_remove_artifacts(const Coordinate& centre_coord, MapPtr view_map, MapPtr fov_map)
{
  std::map<std::string, boost::shared_ptr<Tile> > tile_map = view_map->get_tiles();
  std::map<std::string, boost::shared_ptr<Tile> > fov_tile_map = fov_map->get_tiles();
  std::map<std::string, boost::shared_ptr<Tile> >::iterator t_it;
  
  Log::instance()->log("Centre coord: " + Integer::to_string(centre_coord.first) + "," + Integer::to_string(centre_coord.second));

  for (t_it = tile_map.begin(); t_it != tile_map.end(); t_it++)
  {
    string tile_coords = t_it->first;
    TilePtr current_view_tile = t_it->second;
    
    if (current_view_tile && current_view_tile->get_is_blocking() && (fov_tile_map.find(tile_coords) == fov_tile_map.end()))
    {
      Coordinate c = MapUtils::convert_map_key_to_coordinate(tile_coords);
      
      std::ostringstream ss;
      
      // If we're in the north-west region, and the current tile is north or west of a ground cell in the FOV map, add it to the FOV map.
      if ((c.first < centre_coord.first) && (c.second < centre_coord.second))
      {
        if (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_SOUTH)
        || (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_EAST)))
        {
          Log::instance()->log(tile_coords + " not in list, NW, adjacent to a lit tile.");
          add_point_to_map(c, view_map, fov_map);
        }
      }
      // If we're in the north-east region, and the current tile is north or east of a ground cell in the FOV map, add it to the FOV map.
      else if ((c.first < centre_coord.first) && (c.second > centre_coord.second))
      {
        if (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_SOUTH)
        || (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_WEST)))
        {
          Log::instance()->log(tile_coords + " not in list, NE, adjacent to a lit tile.");
          add_point_to_map(c, view_map, fov_map);
        }
      }
      // If we're in the south-west region, and the current tile is south or west of a ground cell in the FOV map, add it to the FOV map.
      else if ((c.first > centre_coord.first) && (c.second < centre_coord.second))
      {
        if (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_NORTH)
        || (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_EAST)))
        {
          Log::instance()->log(tile_coords + " not in list, SW, adjacent to a lit tile.");
          add_point_to_map(c, view_map, fov_map);
        }
      }
      // If we're in the south-east region, and the current tile is south or east of a ground cell in the FOV map, add it to the FOV map.
      else if ((c.first > centre_coord.first) && (c.second < centre_coord.second))
      {
        if (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_NORTH)
        || (does_adjacent_non_blocking_tile_exist_in_fov_map(fov_map, c, DIRECTION_WEST)))
        {
          Log::instance()->log(tile_coords + " not in list, SE, adjacent to a lit tile.");
          add_point_to_map(c, view_map, fov_map);
        }
      }
    }
  }
}

bool RayCastingFieldOfViewStrategy::does_adjacent_non_blocking_tile_exist_in_fov_map(MapPtr fov_map, const Coordinate& centre_coord, const Direction direction)
{
  Coordinate c_dir = MapUtils::get_new_coordinate(centre_coord, direction);
  TilePtr tile = fov_map->at(c_dir);
  
  if (tile)
  {
    std::ostringstream ss;
    ss << "(" << c_dir.first << "," << c_dir.second << ")";
    Log::instance()->log(ss.str());
  }
  
  return (tile && (tile->get_is_blocking() == false));
}
