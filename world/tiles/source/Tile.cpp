#include "Inventory.hpp"
#include "FeatureFactory.hpp"
#include "InventoryFactory.hpp"
#include "MapFactory.hpp"
#include "Serialize.hpp"
#include "Tile.hpp"

using namespace std;

/*
 ******************************************************************

  Routines for creating and setting/getting attributes of a given
  Tile.

 ******************************************************************/
Tile::Tile()
{
  items = std::make_shared<Inventory>();
  set_default_properties();
  tile_type = TILE_TYPE_UNDEFINED;
  tile_subtype = TILE_TYPE_UNDEFINED;
}

Tile::~Tile()
{
}

bool Tile::operator==(const Tile& tile) const
{
  bool result = true;

  result = result && (illuminated == tile.illuminated);
  result = result && (explored == tile.explored);
  result = result && (viewed == tile.viewed);
  result = result && (tile_type == tile.tile_type);
  result = result && (tile_subtype == tile.tile_subtype);
  result = result && ((creature && tile.creature) || (!creature && !tile.creature));

  result = result && (additional_properties == tile.additional_properties);

  if (result && creature)
  {
    result = result && (*creature == *tile.creature);
  }

  result = result && ((feature && tile.feature) || (!feature && !tile.feature));

  if (result && feature)
  {
    result = result && (*feature == *tile.feature);
  }

  result = result && (items == tile.items);

  result = result && (map_exits.size() == tile.map_exits.size());

  if (result)
  {
    TileExitMap map2 = tile.map_exits;
    TileExitMap::const_iterator t_it = map_exits.begin();
    TileExitMap::const_iterator t_it2 = map2.begin();

    while (t_it != map_exits.end())
    {
      result = result && (*t_it->second == *t_it2->second);

      if (!result) break;

      t_it++;
      t_it2++;
    }
  }

  return result;
}

bool Tile::display_description_on_arrival() const
{
  return false;
}

void Tile::set_extra_description_sid(const string& new_extra_description_sid)
{
  set_additional_property(TileProperties::TILE_PROPERTY_EXTRA_DESCRIPTION_SID, new_extra_description_sid);
}

string Tile::get_extra_description_sid() const
{
  return get_additional_property(TileProperties::TILE_PROPERTY_EXTRA_DESCRIPTION_SID);
}

bool Tile::has_extra_description() const
{
  return has_additional_property(TileProperties::TILE_PROPERTY_EXTRA_DESCRIPTION_SID);
}

void Tile::set_custom_map_id(const string& map_generator_id)
{
  set_additional_property(TileProperties::TILE_PROPERTY_CUSTOM_MAP_ID, map_generator_id);
}

string Tile::get_custom_map_id() const
{
  return get_additional_property(TileProperties::TILE_PROPERTY_CUSTOM_MAP_ID);
}

bool Tile::has_custom_map_id() const
{
  return has_additional_property(TileProperties::TILE_PROPERTY_CUSTOM_MAP_ID);
}

void Tile::set_additional_property(const string& property_name, const string& property_value)
{
  additional_properties[property_name] = property_value;
}

string Tile::get_additional_property(const string& property_name) const
{
  string property_value;

  map<string, string>::const_iterator t_it = additional_properties.find(property_name);
  if (t_it != additional_properties.end())
  {
    property_value = t_it->second;
  }

  return property_value;
}

size_t Tile::remove_additional_property(const string& property_name)
{
  return additional_properties.erase(property_name);
}

map<string, string> Tile::get_additional_properties() const
{
  return additional_properties;
}

bool Tile::has_additional_property(const std::string& property_name) const
{
  string property_value = get_additional_property(property_name);
  return (!property_value.empty());
}

void Tile::set_default_properties()
{
  illuminated = true;
  explored    = false;
  viewed      = false;
}

void Tile::set_illuminated(bool new_illuminated)
{
  illuminated = new_illuminated;
}

bool Tile::get_illuminated() const
{
  return illuminated;
}

// The conditions are broken up for easier debugging.
//
// A tile should only be blocking from the perspective of a particular
// incorporeal creature (ghost, spirit, etc) if the tile has another
// creature present.
bool Tile::get_is_blocking(CreaturePtr perspective_creature) const
{
  bool tile_blocking = false;
  bool perspective_creature_incorporeal = perspective_creature && perspective_creature->has_status(StatusIdentifiers::STATUS_ID_INCORPOREAL);

  if (get_movement_multiplier() == 0 && !perspective_creature_incorporeal)
  {
    tile_blocking = true;
  }

  if (feature && feature->get_is_blocking() && !perspective_creature_incorporeal)
  {
    tile_blocking = true;
  }
  
  if (creature && perspective_creature 
               && (creature->get_id() != perspective_creature->get_id()))
  {
    // Check to see if the searching creature is hostile to this creature.
    // If the creature's not hostile, consider the tile blocking; otherwise,
    // ues whatever the current value of tile_blocking is (based on movement,
    // features, etc).
    if (!perspective_creature->get_decision_strategy()->get_threats_ref().has_threat(creature->get_id()).first)
    {
      tile_blocking = true;
    }
  }

  return tile_blocking;
}

int Tile::get_movement_multiplier() const
{
  return 1;
}

void Tile::set_explored(bool new_explored)
{
  explored = new_explored;
}

bool Tile::get_explored() const
{
  return explored;
}

void Tile::set_viewed(bool new_viewed)
{
  viewed = new_viewed;
}

bool Tile::get_viewed() const
{
  return viewed;
}

bool Tile::has_feature() const
{
  return (feature != nullptr);
}

bool Tile::has_blocking_feature() const
{
  return (feature && feature->get_is_blocking());
}

void Tile::set_feature(FeaturePtr new_feature)
{
  feature = new_feature;
}

FeaturePtr Tile::get_feature()
{
  return feature;
}

bool Tile::has_creature() const
{
  return (creature != NULL);
}

void Tile::set_creature(const CreaturePtr& new_creature)
{
  creature = new_creature;
  
  if (creature->get_is_player())
  {
    set_explored(true);
  }
}

void Tile::remove_creature()
{
  creature.reset();
}

CreaturePtr Tile::get_creature() const
{
  return creature;
}

IInventoryPtr Tile::get_items()
{
  return items;
}


TileType Tile::get_tile_type() const
{
  return tile_type;
}

// All tiles are assumed to be ground tiles, unless the derived class declares otherwise.
TileSuperType Tile::get_tile_super_type() const
{
  return TILE_SUPER_TYPE_GROUND;
}

void Tile::set_tile_subtype(const TileType new_tile_subtype)
{
  tile_subtype = new_tile_subtype;
}

TileType Tile::get_tile_subtype() const
{
  return tile_subtype;
}

TileExitMap& Tile::get_tile_exit_map_ref()
{
  return map_exits;
}

bool Tile::get_dangerous(CreaturePtr creature) const
{
  return false;
}

string Tile::get_danger_confirmation_sid() const
{
  string empty_sid;
  return empty_sid;
}

float Tile::get_piety_loss_multiplier() const
{
  float piety_mult = 1.0;

  if (feature != nullptr)
  {
    piety_mult = feature->get_piety_loss_multiplier();
  }

  return piety_mult;
}

// Transform a tile from an original tile type.  The new tile (this object)
// will be assumed to have the new tile type and subtype, but won't have
// any of the original's required values: creature, inventory, exits,
// certain flags, etc.  Copy those values over to this tile.
void Tile::transformFrom(std::shared_ptr<Tile> original_tile)
{
  if (original_tile != nullptr)
  {
    // Keep the tile type and subtype.
    // Copy everything else.
    TileType orig_tt, orig_tst;
    
    orig_tt = original_tile->get_tile_type();
    orig_tst = original_tile->get_tile_subtype();

    *this = *original_tile;

    tile_type = orig_tt;
    tile_subtype = orig_tst;
  }
}

bool Tile::serialize(ostream& stream) const
{
  Serialize::write_bool(stream, illuminated);
  Serialize::write_bool(stream, explored);
  Serialize::write_bool(stream, viewed);
  Serialize::write_enum(stream, tile_type);
  Serialize::write_enum(stream, tile_subtype);

  Serialize::write_string_map(stream, additional_properties);

  if (creature)
  {
    Serialize::write_class_id(stream, creature->get_class_identifier());
    creature->serialize(stream);
  }
  else
  {
    Serialize::write_class_id(stream, CLASS_ID_NULL);
  }

  if (feature)
  {
    Serialize::write_class_id(stream, feature->get_class_identifier());
    feature->serialize(stream);
  }
  else
  {
    Serialize::write_class_id(stream, CLASS_ID_NULL);
  }
  
  Serialize::write_class_id(stream, items->get_class_identifier());
  items->serialize(stream);

  Serialize::write_size_t(stream, map_exits.size());

  for (const TileExitMap::value_type& tile_exit_map_pair : map_exits)
  {
    Direction d = tile_exit_map_pair.first;
    MapExitPtr map_exit = tile_exit_map_pair.second;

    Serialize::write_enum(stream, d);

    if (map_exit)
    {
      Serialize::write_class_id(stream, map_exit->get_class_identifier());
      map_exit->serialize(stream);
    }
    else
    {
      Serialize::write_class_id(stream, CLASS_ID_NULL);
    }
  }

  return true;
}

bool Tile::deserialize(istream& stream)
{
  Serialize::read_bool(stream, illuminated);
  Serialize::read_bool(stream, explored);
  Serialize::read_bool(stream, viewed);
  Serialize::read_enum(stream, tile_type);
  Serialize::read_enum(stream, tile_subtype);

  Serialize::read_string_map(stream, additional_properties);

  ClassIdentifier creature_clid;
  Serialize::read_class_id(stream, creature_clid);

  if (creature_clid != CLASS_ID_NULL)
  {
    creature = std::make_shared<Creature>();
    creature->deserialize(stream);
  }

  ClassIdentifier feature_clid;
  Serialize::read_class_id(stream, feature_clid);

  if (feature_clid != CLASS_ID_NULL)
  {
    FeaturePtr feature = FeatureFactory::create_feature(feature_clid);
    if (!feature) return false;
    if (!feature->deserialize(stream)) return false;
    set_feature(feature);
  }

  ClassIdentifier items_class_id;
  Serialize::read_class_id(stream, items_class_id);

  items = InventoryFactory::create_inventory(items_class_id);
  items->deserialize(stream);

  size_t map_size;
  Serialize::read_size_t(stream, map_size);

  map_exits.clear();

  for (unsigned int i = 0; i < map_size; i++)
  {
    Direction d = DIRECTION_NORTH; // just a random default value - otherwise, compiler warnings.
    Serialize::read_enum(stream, d);

    ClassIdentifier map_exit_clid;
    Serialize::read_class_id(stream, map_exit_clid);
    if (map_exit_clid != CLASS_ID_NULL)
    {
      MapExitPtr current_dir_map_exit = MapFactory::create_map_exit();
      current_dir_map_exit->deserialize(stream);
      map_exits.insert(make_pair(d, current_dir_map_exit));
    }
  }

  return true;
}

ClassIdentifier Tile::internal_class_identifier() const
{
  return CLASS_ID_TILE;
}

#ifdef UNIT_TESTS
#include "unit_tests/Tile_test.cpp"
#endif
