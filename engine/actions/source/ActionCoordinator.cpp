#include "ActionCoordinator.hpp"
#include "Serialize.hpp"

using namespace std;

bool ActionCoordinator::operator==(const ActionCoordinator& ac) const
{
  bool result = true;

  result = result && (creature_action_order == ac.creature_action_order);
  result = result && (current_map_id == ac.current_map_id);

  return result;
}

// If the new map ID is different than the current one, then clear the action coordinator, rebuild it, and set the
// current ID to the new ID.
bool ActionCoordinator::reset_if_necessary(const bool map_is_permanent, const string& new_map_id, const map<string, CreaturePtr>& map_creatures)
{
  bool ac_reset = (new_map_id != current_map_id) || (!map_is_permanent);

  if (ac_reset)
  {
    clear();
    set(map_creatures);
    current_map_id = new_map_id;
  }

  return ac_reset;
}

void ActionCoordinator::set(const map<string, CreaturePtr>& creatures)
{
  creature_action_order.clear();
  
  for (map<string, CreaturePtr>::const_iterator c_it = creatures.begin(); c_it != creatures.end(); c_it++)
  {
    if (c_it->second)
    {
      ActionCost ac; // Default initially - will be 0.
      string creature_id = c_it->first;
      creature_action_order.insert(make_pair(ac, creature_id));
    }
  }
}

void ActionCoordinator::add(const ActionCost& ac, const string& creature_id)
{
  creature_action_order.insert(make_pair(ac, creature_id));
}

void ActionCoordinator::clear()
{
  creature_action_order.clear();
}

void ActionCoordinator::update_actions()
{
  // Remove the first item from the map, and update the remaining costs based on the current minus the cost of the first.
  multimap<ActionCost, string> new_action_ordering;

  if (!creature_action_order.empty())
  {
    ActionOrderIterator ac_it = creature_action_order.begin();
    pair<ActionCost, string> next_action = *(ac_it);
    creature_action_order.erase(ac_it);
    
    ActionCostValue next_action_cost_value = next_action.first.get_cost();
    
    for (ActionOrderIterator reorder_it = creature_action_order.begin(); reorder_it != creature_action_order.end(); reorder_it++)
    {
      ActionCost ac = reorder_it->first;
      ac.set_cost(ac.get_cost() - next_action_cost_value);
      
      new_action_ordering.insert(make_pair(ac, reorder_it->second));
    }
  }
  
  creature_action_order = new_action_ordering;
}

// Return the creature ID of the next creature to act.
string ActionCoordinator::get_next_creature_id() const
{
  string next_creature_id;

  if (!creature_action_order.empty())
  {
    next_creature_id = creature_action_order.begin()->second;
  }
  
  return next_creature_id;
}

// Get the cost of the next action - used to determine how much
// to increment the world's calendar by, among other things.
ActionCost ActionCoordinator::get_current_action_cost() const
{
  ActionCost next_action_cost;
  
  if (!creature_action_order.empty())
  {
    next_action_cost = creature_action_order.begin()->first;
  }
  
  return next_action_cost;
}

// Return the creature ID of the next creature to act,
// and then remove the first item from the map via update()
string ActionCoordinator::get_next_creature_id_and_update_actions()
{
  string creature_id = get_next_creature_id();
  update_actions();
  return creature_id;
}

// Check to see if there are any creatures in the queue.
bool ActionCoordinator::has_actions() const
{
  return (!creature_action_order.empty());
}

string ActionCoordinator::get_current_map_id() const
{
  return current_map_id;
}

bool ActionCoordinator::serialize(std::ostream& stream) const
{
  Serialize::write_size_t(stream, creature_action_order.size());

  for(const CreatureActionMultimap::value_type& action_pair : creature_action_order)
  {
    ActionCost cost = action_pair.first;
    cost.serialize(stream);
    Serialize::write_string(stream, action_pair.second);
  }

  Serialize::write_string(stream, current_map_id);

  return true;
}

bool ActionCoordinator::deserialize(std::istream& stream)
{
  size_t multimap_size;
  Serialize::read_size_t(stream, multimap_size);

  creature_action_order.clear();

  for (unsigned int i = 0; i < multimap_size; i++)
  {
    ActionCost cost;
    cost.deserialize(stream);

    string id;
    Serialize::read_string(stream, id);

    creature_action_order.insert(make_pair(cost, id));
  }

  Serialize::read_string(stream, current_map_id);

  return true;
}

ClassIdentifier ActionCoordinator::internal_class_identifier() const
{
  return ClassIdentifier::CLASS_ID_ACTION_COORDINATOR;
}

#ifdef UNIT_TESTS
#include "unit_tests/ActionCoordinator_test.cpp"
#endif

