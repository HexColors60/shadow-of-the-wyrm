#include "ClassManager.hpp"
#include "Game.hpp"

using namespace std;

ClassManager::ClassManager()
{
}

Class* ClassManager::get_class(const string& class_id)
{
  Class* cur_class = nullptr;
  Game& game = Game::instance();

  const ClassMap& classes = game.get_classes_ref();
  auto& c_it = classes.find(class_id);

  if (c_it != classes.end())
  {
    cur_class = c_it->second.get();
  }

  return cur_class;
}

// Get the character's title by looking up the title from the class's
// title map, based on the creature's current level.
string ClassManager::get_title(CreaturePtr creature)
{
  string title_sid;

  if (creature)
  {
    Class* cr_class = get_class(creature->get_class_id());
    int cr_level = creature->get_level().get_current();
    map<int, string> titles = cr_class->get_titles();

    // Because the levels are at odd intervals (1, 10, 20, 30, 45) that are
    // defined in the XML rather than at hard-coded, walk over the
    // map until a level greater than the creature's is found.  The previous
    // title SID will be used to look up the creature's title.
    for (const auto& t_pair : titles)
    {
      int level = t_pair.first;

      if (level > cr_level)
      {
        break;
      }
      else
      {
        title_sid = t_pair.second;
      }
    }
  }

  return StringTable::get(title_sid);
}