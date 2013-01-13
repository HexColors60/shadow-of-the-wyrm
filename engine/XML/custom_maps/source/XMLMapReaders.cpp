#include <boost/foreach.hpp>
#include "CreatureFactory.hpp"
#include "Game.hpp"
#include "ItemManager.hpp"
#include "MapUtils.hpp"
#include "XMLMapCoordinateReader.hpp"
#include "XMLMapExitReader.hpp"
#include "XMLMapFeatureReaders.hpp"
#include "XMLMapReaders.hpp"
#include "XMLMapTilesReader.hpp"
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
    XMLNode exits_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "Exits");
    XMLNode features_node = XMLUtils::get_next_element_by_local_name(custom_map_node, "Features");

    string map_id = XMLUtils::get_attribute_value(custom_map_node, "id");
    MapType map_type = static_cast<MapType>(XMLUtils::get_child_node_int_value(custom_map_node, "MapType"));

    Dimensions dim = parse_dimensions(dimensions_node);

    XMLMapTilesReader tiles_reader;
    TilesContainer tiles = tiles_reader.parse_tiles(tiles_node, dim.get_y(), dim.get_x());

    XMLMapCoordinateReader coord_reader;
    Coordinate player_start_location = coord_reader.parse_coordinate(player_start_node);

    custom_map = MapPtr(new Map(dim));
    
    custom_map->set_map_id(map_id);
    custom_map->set_map_type(map_type);
    custom_map->set_tiles(tiles);
    custom_map->set_permanent(true); // custom maps are always permanent.
    custom_map->add_or_update_location(WorldMapLocationTextKeys::CURRENT_PLAYER_LOCATION, player_start_location);

    parse_initial_placements(initial_placements_node, custom_map);

    XMLMapExitReader exit_reader;
    exit_reader.parse_exits(exits_node, custom_map);

    XMLMapFeaturesReader features_reader;
    features_reader.parse_features(features_node, custom_map);

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

// Parse the initial placement of items and creatures.
void XMLMapReader::parse_initial_placements(const XMLNode& initial_placements_node, MapPtr map)
{
  if (!initial_placements_node.is_null())
  {
    XMLNode creatures_node = XMLUtils::get_next_element_by_local_name(initial_placements_node, "Creatures");
    parse_initial_creature_placements(creatures_node, map);

    XMLNode items_node = XMLUtils::get_next_element_by_local_name(initial_placements_node, "Items");
    parse_initial_item_placements(items_node, map);
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

      XMLMapCoordinateReader coord_reader;
      Coordinate coord = coord_reader.parse_coordinate(coord_node);

      // Place the specified creature on the map.
      Game* game = Game::instance();

      if (game)
      {
        CreaturePtr creature = CreatureFactory::create_by_creature_id(game->get_action_manager_ref(), id);

        TilePtr placement_tile = map->at(coord);

        if (creature && placement_tile)
        {
          if (MapUtils::is_tile_available_for_creature(creature, placement_tile))
          {
            MapUtils::add_or_update_location(map, creature, coord);
          }
        }
      }
    }
  }
}

// Parse the initial placement of items, and place them at the specified coordinates.
void XMLMapReader::parse_initial_item_placements(const XMLNode& items_node, MapPtr map)
{
  if (!items_node.is_null())
  {
    Game* game = Game::instance();

    if (game)
    {
      vector<XMLNode> placement_nodes = XMLUtils::get_elements_by_local_name(items_node, "Placement");

      BOOST_FOREACH(const XMLNode& item_node, placement_nodes)
      {
        string id = XMLUtils::get_child_node_value(item_node, "ID");
        XMLNode coord_node = XMLUtils::get_next_element_by_local_name(item_node, "Coord");
        
        XMLMapCoordinateReader coord_reader;
        Coordinate c = coord_reader.parse_coordinate(coord_node);

        // Create the item, and set it on the specified coordinate.
        ItemPtr item = ItemManager::create_item(id);
        TilePtr tile = map->at(c);

        if (item && tile)
        {
          tile->get_items().add(item);
        }
      }
    }
  }
}

