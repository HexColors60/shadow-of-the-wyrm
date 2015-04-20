#pragma once
#include "IActionManager.hpp"

class ActionManager;

class DropAction : public IActionManager
{
  public:
    ActionCostValue drop(CreaturePtr creature, ActionManager * const am);
    
    ActionCostValue get_action_cost_value(CreaturePtr creature) const override;
  
  protected:
    void handle_world_drop(CreaturePtr creature);
    void handle_item_dropped_message(CreaturePtr creature, ItemPtr item);
    void handle_no_item_dropped(CreaturePtr creature);
    void handle_invalid_drop_quantity(CreaturePtr creature);
    
    ActionCostValue do_drop(CreaturePtr creature, MapPtr current_map, ItemPtr item);
    uint get_drop_quantity(CreaturePtr creature, const uint max_quantity) const;
};
