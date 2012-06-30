#include <boost/make_shared.hpp>
#include "CombatManager.hpp"
#include "Conversion.hpp"
#include "Game.hpp"
#include "Log.hpp"
#include "MessageManager.hpp"
#include "MovementManager.hpp"
#include "MapExitUtils.hpp"
#include "MapUtils.hpp"
#include "TerrainGeneratorFactory.hpp"

using namespace std;
using boost::make_shared;

MovementManager::MovementManager()
{
}

MovementManager::~MovementManager()
{
}

ActionCostValue MovementManager::move(CreaturePtr creature, const Direction direction)
{
  ActionCostValue movement_success = 0;

  Game* game = Game::instance();

  if (game && creature)
  {
    MapPtr map = game->get_current_map();
    
    // Get the creature's location from the map
    Coordinate creature_location = map->get_location(creature->get_id());

    // Get the old tile
    TilePtr creatures_old_tile = map->at(creature_location.first, creature_location.second);

    // Is the proposed movement valid?
    // If it is not, and there is no map exit, and the creature is the player, display a message.
    // Otherwise, move the creature.
    if (!MapUtils::is_valid_move(map->size(), creature_location, direction))
    {
      movement_success = move_off_map(creature, map, creatures_old_tile);
    }
    // Otherwise, it's a regular move within the current map.
    else
    {
      Coordinate new_coords = MapUtils::get_new_coordinate(creature_location, direction);
      TilePtr creatures_new_tile = map->at(new_coords.first, new_coords.second);
      
      movement_success = move_within_map(creature, map, creatures_old_tile, creatures_new_tile, new_coords);
    }
  }
  
  return movement_success;
}

ActionCostValue MovementManager::move_off_map(CreaturePtr creature, MapPtr map, TilePtr creatures_old_tile)
{
  ActionCostValue movement_success = 0;

  Game* game = Game::instance();
  MessageManager* manager = MessageManager::instance();  
  MapExitPtr map_exit = map->get_map_exit();

  if (game && manager)
  {
    if (!MapUtils::can_exit_map(map_exit))
    {
      if (creature->get_is_player())
      { 
        string movement_message = StringTable::get(MovementTextKeys::ACTION_MOVE_OFF_WORLD_MAP);

        manager->add_new_message(movement_message);
        manager->send();
      }
    }
    else
    {
      if (creature->get_is_player())
      {
        string leave_area = TextMessages::get_confirmation_message(TextKeys::DECISION_LEAVE_AREA);
        game->display->confirm(leave_area);
      
        if (creature->get_decision_strategy()->get_confirmation())
        {
          move_to_new_map(map_exit);
          movement_success = get_action_cost_value();
        }
      
        // Regardless of whether we leave the map or not, clear the messages, so the text doesn't hang around.
        game->display->clear_messages();
      }
      // It's an NPC leaving the map - display the exit message.
      else
      {
        // Remove from tile and from map's creatures.
        creatures_old_tile->remove_creature();
        map->remove_creature(creature->get_id());
        
        string npc_exit_message = TextMessages::get_npc_escapes_message(StringTable::get(creature->get_description_sid()));
        manager->add_new_message(npc_exit_message);
        manager->send();
        
        movement_success = get_action_cost_value();
        
        // JCD FIXME: Might cause problems later with many turns.
      } 
    }
  }
  
  return movement_success;
}

ActionCostValue MovementManager::move_within_map(CreaturePtr creature, MapPtr map, TilePtr creatures_old_tile, TilePtr creatures_new_tile, const Coordinate& new_coords)
{
  ActionCostValue movement_success = 0;

  Game* game = Game::instance();
  MessageManager* manager = MessageManager::instance();
  
  if (game && manager && creatures_new_tile)
  {
    if (creatures_new_tile->get_is_blocking())
    {
      // Do nothing, and return false.
    }
    else if (MapUtils::is_creature_present(creatures_new_tile))
    {
      movement_success = 0;
      
      // Do the necessary checks here to determine whether to attack...
      CreaturePtr adjacent_creature = creatures_new_tile->get_creature();
      
      // Sanity check
      if (adjacent_creature)
      {
        CombatManager cm;
        movement_success = cm.attack(creature, adjacent_creature);
      }
    }
    else if (MapUtils::is_blocking_feature_present(creatures_new_tile))
    {
      string blocked = StringTable::get(ActionTextKeys::ACTION_MOVEMENT_BLOCKED);
      manager->add_new_message(blocked);
      manager->send();
      
      movement_success = 0;
    }
    else
    {
      if (confirm_move_to_tile_if_necessary(creature, creatures_old_tile, creatures_new_tile))
      {
        // Update the map info
        MapUtils::add_or_update_location(map, creature, new_coords, creatures_old_tile);
        TilePtr new_tile = MapUtils::get_tile_for_creature(map, creature);
        add_tile_related_messages(creature, manager, new_tile);
        movement_success = get_action_cost_value();
      }
    }
  }
  
  return movement_success;
}

// Confirm if moving to a potentially dangerous tile from a safe one.
bool MovementManager::confirm_move_to_tile_if_necessary(CreaturePtr creature, TilePtr creatures_old_tile, TilePtr creatures_new_tile)
{
  MessageManager* manager = MessageManager::instance();
  
  // If there get to be enough of these, break these out into a map or a class or something.
  // Probably should break out of MapUtils...
  if (manager && MapUtils::is_moving_from_land_type_tile_to_water_type_tile(creatures_old_tile, creatures_new_tile))
  {
    if (creature->get_is_player())
    { 
      string swim = TextMessages::get_confirmation_message(TextKeys::DECISION_JUMP_INTO_WATER);
      manager->add_new_confirmation_message(swim);
    }
    
    bool confirmation = (creature->get_decision_strategy()->get_confirmation());
    manager->clear_if_necessary();
    return confirmation;
  }
  else
  {
    return true;
  }
}

void MovementManager::move_to_new_map(MapPtr new_map)
{
  Game* game = Game::instance();
  
  if (game && new_map)
  {
    game->set_current_map(new_map);
    game->reload_map();    
  }
}

void MovementManager::move_to_new_map(MapExitPtr map_exit)
{
  Game* game = Game::instance();
  
  if (game && map_exit)
  {
    if (map_exit->is_using_map_id())
    {
      string new_map_id = map_exit->get_map_id();
      MapPtr new_map = game->map_registry.get_map(new_map_id);
      
      move_to_new_map(new_map);
    }
    else
    {
      // A random terrain map.
      // JCD FIXME fill this in later.
    }
  }
}

ActionCostValue MovementManager::ascend(CreaturePtr creature)
{
  ActionCostValue ascend_success = 0;
  
  if (creature->get_is_player())
  {
    Game* game = Game::instance();
    MessageManager* manager = MessageManager::instance();
    
    if (game)
    {
      MapPtr current_map = game->get_current_map();

      // Otherwise, check to see if the creature is on a tile with DIRECTION_UP defined.
      TilePtr current_tile = MapUtils::get_tile_for_creature(current_map, creature);
      TileExitMap& tile_exit_map = current_tile->get_tile_exit_map_ref();
      TileExitMap::iterator t_it = tile_exit_map.find(DIRECTION_UP);
      
      MapExitPtr map_exit;
      
      if (t_it != tile_exit_map.end())
      {
        map_exit = t_it->second;        
      }
  
      if (map_exit)
      {
        move_to_new_map(map_exit);
        
        // If the tile we've moved to has any items, notify the player, if the creature's a player.
        MapPtr new_map = game->get_current_map();
        TilePtr creatures_current_tile = MapUtils::get_tile_for_creature(new_map, creature);
        add_message_about_items_on_tile_if_necessary(creature, manager, creatures_current_tile);
        
        ascend_success = get_action_cost_value();
      }
      else
      {
        // This is so that it's easy to find/replace this later, once I add message managers
        // per-creature
        if (creature->get_is_player())
        {
          // Let the player know there is no exit.
          string no_exit = StringTable::get(MovementTextKeys::ACTION_MOVE_NO_EXIT);
          
          manager->add_new_message(no_exit);
          manager->send();                
        }
      }
    }
  }
  else
  {
    Log* log = Log::instance();
    log->log("Trying to ascend with non-player creature.  Not supported yet.");
  }
  
  return ascend_success;
}

ActionCostValue MovementManager::descend(CreaturePtr creature)
{
  ActionCostValue descend_success = 0;
 
  MessageManager* manager = MessageManager::instance();
  
  if (creature->get_is_player())
  {
    // If we're on the world map, we can always descend.
    Game* game = Game::instance();
    
    if (game)
    {
      MapPtr map = game->get_current_map();

      if (map)
      {
        // Look up the creature in the map
        Coordinate c = map->get_location(creature->get_id());
      
        // Get the creature's tile's MapExitPtr
        TilePtr tile = map->at(c);
        
        if (tile)
        {
          TileExitMap& exit_map = tile->get_tile_exit_map_ref();
          TileExitMap::const_iterator t_it = exit_map.find(DIRECTION_DOWN);
        
          // If there is an exit in the down direction, do the appropriate action.
          if (t_it != exit_map.end())
          {
            // JCD FIXME Fill this in later.
            descend_success = get_action_cost_value();
          }
          // If it's null, check to see if we're on the world map.
          else
          {
            MapType map_type = map->get_map_type();
            
            if (map_type == MAP_TYPE_WORLD)
            {
              TileType tile_type     = tile->get_tile_type();
              TileType tile_subtype  = tile->get_tile_subtype();
              GeneratorPtr generator = TerrainGeneratorFactory::create_generator(tile, map->get_map_id(), tile_type, tile_subtype);
              
              if (generator)
              {
                // - Generate the map.
                uint danger_level = creature->get_level().get_current();
                MapPtr new_map = generator->generate_and_initialize(danger_level);
                
                // - Set the map's MapExitPtr to point to the previous map.
                //   But only if it's an overworld map.
                if (new_map->get_map_type() == MAP_TYPE_OVERWORLD)
                {
                  MapExitUtils::add_exit_to_map(new_map, map->get_map_id());
                }
                
                // If the map has a last known player location (e.g., up staircase),
                // use that.  Otherwise, start at 0,0.  JCD FIXME THAT WON'T ALWAYS HOLD!
                string player_loc = WorldMapLocationTextKeys::CURRENT_PLAYER_LOCATION;
                Coordinate starting_coords;
                
                if (new_map->has_location(player_loc))
                {
                  starting_coords = new_map->get_location(player_loc);
                }
                else
                {
                  starting_coords.first = 0;
                  starting_coords.second = 0;
                }
                
                MapUtils::add_or_update_location(new_map, creature, starting_coords);
                TilePtr new_creature_tile = new_map->at(starting_coords);

                move_to_new_map(new_map);
                
                manager->add_new_message(TextMessages::get_area_entrance_message_given_terrain_type(tile_type));
                add_tile_related_messages(creature, manager, new_creature_tile);
                
                descend_success = get_action_cost_value();
              }
            }
            else
            {
              // This is so that it's easy to find/replace this later, once I add message managers
              // per-creature
              if (creature->get_is_player())
              {
                // Let the player know there is no exit.
                string no_exit = StringTable::get(MovementTextKeys::ACTION_MOVE_NO_EXIT);
                
                manager->add_new_message(no_exit);
                manager->send();                
              }
            }
          }  
        }
      }
    }
  }
  else
  {
    Log* log = Log::instance();
    log->log("Trying to descend with non-player creature.  Not supported yet.");    
  }
  
  return descend_success;
}

// Add any messages after moving to a particular tile:
// - Should a message be displayed about the tile automatically? (staircases, etc)
//       If so, add it.
// - Are there any items on the tile?
//       If so, add the appropriate message.
void MovementManager::add_tile_related_messages(const CreaturePtr& creature, MessageManager* manager, TilePtr tile)
{
  add_message_about_tile_if_necessary(creature, manager, tile);
  add_message_about_items_on_tile_if_necessary(creature, manager, tile);

  manager->send();
}

// Add a message about the tile if necessary.
void MovementManager::add_message_about_tile_if_necessary(const CreaturePtr& creature, MessageManager* manager, TilePtr tile)
{
  if (creature && tile && creature->get_is_player())
  {
    if (tile->display_description_on_arrival())
    {
      string tile_desc = StringTable::get(tile->get_tile_description_sid());
      manager->add_new_message(tile_desc);
    }
  }
}

// Add a message if the creature is the player, and if there are items on
// the tile.
void MovementManager::add_message_about_items_on_tile_if_necessary(const CreaturePtr& creature, MessageManager* manager, TilePtr tile)
{
  if (creature && creature->get_is_player())
  {
    Inventory& tile_items = tile->get_items();
    
    if (!tile_items.empty())
    {
      string item_message;
      
      // One item
      if (tile_items.size() == 1)
      {
        ItemPtr item_on_tile = tile_items.at(0);
        
        if (item_on_tile)
        {
          item_message = TextMessages::get_item_on_ground_description_message(StringTable::get(item_on_tile->get_usage_description_sid()));
        }
      }
      // Multiple items
      else
      {
        item_message = StringTable::get(MovementTextKeys::ITEMS_ON_TILE);
      }
      
      // Send the message
      if (!item_message.empty())
      {
        manager->add_new_message(item_message);
      }
    }
  }
}

ActionCostValue MovementManager::get_action_cost_value() const
{
  // JCD FIXME
  return 1;
}
