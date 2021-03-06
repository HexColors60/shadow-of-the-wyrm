#include "Mortuary.hpp"
#include "Serialize.hpp"

using namespace std;

Mortuary::Mortuary()
: max_level_difference({ -1, "" })
{
}

bool MortuaryEntry::operator==(const MortuaryEntry& me) const
{
  bool result = true;

  result = result && (count == me.count);
  result = result && (max == me.max);
  result = result && (short_desc_sid == me.short_desc_sid);

  return result;
}

bool Mortuary::operator==(const Mortuary& m) const
{
  bool result = true;

  result = result && (creatures_killed == m.creatures_killed);
  result = result && (max_level_difference == m.max_level_difference);

  return result;
}

// Update the death toll.
void Mortuary::add_creature_kill(const string& creature_id, const string& short_desc_sid, const bool is_unique)
{
  MortuaryCountMap::iterator m_it = creatures_killed.find(creature_id);

  if (m_it != creatures_killed.end())
  {
    MortuaryEntry& me = creatures_killed[creature_id];
    me.count += 1;
  }
  else
  {
    MortuaryEntry me;

    me.max = (is_unique) ? 1 : -1;
    me.count = 1;
    me.short_desc_sid = short_desc_sid;

    creatures_killed.insert(make_pair(creature_id, me));
  }
}

MortuaryEntry Mortuary::get_entry(const string& creature_id) const
{
  MortuaryEntry me;

  auto c_it = creatures_killed.find(creature_id);

  if (c_it != creatures_killed.end())
  {
    me = c_it->second;
  }

  return me;
}

MortuaryCountMap Mortuary::get_creatures_killed() const
{
  return creatures_killed;
}

// Get the number of a particular type of creature that have been killed.
int Mortuary::get_num_creature_killed(const string& creature_id) const
{
  uint num_killed = 0;

  MortuaryCountMap::const_iterator m_it = creatures_killed.find(creature_id);

  if (m_it != creatures_killed.end())
  {
    num_killed = m_it->second.count;
  }

  return static_cast<int>(num_killed);
}

// Get the total number of creatures killed.
int Mortuary::get_num_creatures_killed() const
{
  uint num_killed = 0;

  for (const MortuaryCountMap::value_type& pair : creatures_killed)
  {
    num_killed += pair.second.count;
  }

  return static_cast<int>(num_killed);
}

// Get the number of uniques killed.
int Mortuary::get_num_uniques_killed() const
{
  uint num_killed = 0;

  for (const MortuaryCountMap::value_type& pair : creatures_killed)
  {
    if (pair.second.max == 1)
    {
      num_killed += pair.second.count;
    }
  }

  return static_cast<int>(num_killed);
}

void Mortuary::set_max_level_difference(const pair<int, string>& new_max_level_difference)
{
  max_level_difference = new_max_level_difference;
}

pair<int, string> Mortuary::get_max_level_difference() const
{
  return max_level_difference;
}

bool Mortuary::serialize(ostream& stream) const
{
  size_t ck_size = creatures_killed.size();
  Serialize::write_size_t(stream, ck_size);

  if (ck_size > 0)
  {
    for (const MortuaryCountMap::value_type& pair : creatures_killed)
    {
      Serialize::write_string(stream, pair.first);
      Serialize::write_int(stream, pair.second.max);
      Serialize::write_int(stream, pair.second.count);
      Serialize::write_string(stream, pair.second.short_desc_sid);
    }
  }

  Serialize::write_int(stream, max_level_difference.first);
  Serialize::write_string(stream, max_level_difference.second);

  return true;
}

bool Mortuary::deserialize(istream& stream)
{
  size_t ck_size = 0;
  Serialize::read_size_t(stream, ck_size);

  if (ck_size > 0)
  {
    creatures_killed.clear();

    for (uint i = 0; i < ck_size; i++)
    {
      string creature_id;
      Serialize::read_string(stream, creature_id);

      int creature_max = 0;
      Serialize::read_int(stream, creature_max);

      int creature_count = 0;
      Serialize::read_int(stream, creature_count);

      string short_desc_sid;
      Serialize::read_string(stream, short_desc_sid);

      MortuaryEntry me;

      me.max = creature_max;
      me.count = creature_count;
      me.short_desc_sid = short_desc_sid;

      creatures_killed.insert(make_pair(creature_id, me));
    }
  }

  Serialize::read_int(stream, max_level_difference.first);
  Serialize::read_string(stream, max_level_difference.second);

  return true;
}

ClassIdentifier Mortuary::internal_class_identifier() const
{
  return ClassIdentifier::CLASS_ID_MORTUARY;
}

#ifdef UNIT_TESTS
#include "unit_tests/Mortuary_test.cpp"
#endif

