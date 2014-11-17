#include <boost/algorithm/string/replace.hpp>
#include "CorpseTextKeys.hpp"
#include "StringTable.hpp"

using namespace std;

const string CorpseTextKeys::CORPSE_MESSAGE = "CORPSE_MESSAGE";
const string CorpseTextKeys::SKIN_MESSAGE = "SKIN_MESSAGE";

CorpseTextKeys::CorpseTextKeys()
{
}

CorpseTextKeys::~CorpseTextKeys()
{
}

// Create the corpse description.  This will be something like 
// "A goblin corpse".
string CorpseTextKeys::get_corpse_description(const string& creature_desc)
{
  string corpse_desc;

  corpse_desc = StringTable::get(CORPSE_MESSAGE);
  boost::replace_first(corpse_desc, "%s", creature_desc);

  return corpse_desc;
}

string CorpseTextKeys::get_skin_description(const string& creature_desc)
{
  string corpse_desc;

  corpse_desc = StringTable::get(SKIN_MESSAGE);
  boost::replace_first(corpse_desc, "%s", creature_desc);

  return corpse_desc;
}
