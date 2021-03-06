#include <string>
#include "ClassSelectionScreen.hpp"
#include "RaceSelectionScreen.hpp"
#include "Game.hpp"
#include "OptionsComponent.hpp"
#include "PromptTextKeys.hpp"
#include "TextComponent.hpp"
#include "TextKeys.hpp"

using namespace std;

ClassSelectionScreen::ClassSelectionScreen(DisplayPtr new_display, const string& synop)
: Screen(new_display), creature_synopsis(synop)
{
  initialize();
}

// Initialize the class selection screen.  Classes must have been read in, and set into the Game
// instance at this point.
//
// The Screen will return an int based on the user's selection.  This will map to a race_id, which will then map to a
// Class*
void ClassSelectionScreen::initialize()
{
  Game& game_instance = Game::instance();
  const ClassMap& classes = game_instance.get_classes_ref();
  ostringstream synop;

  if (!creature_synopsis.empty())
  {
    synop << "[" << creature_synopsis << "]";
  }

  TextComponentPtr class_selection_text = std::make_shared<TextComponent>(StringTable::get(TextKeys::SELECT_CLASS));

  // Each option represents a player-selectable class.
  OptionsComponentPtr options = std::make_shared<OptionsComponent>();

  int current_id = 0;
  for (auto& classes_pair : classes)
  {
    string class_id = classes_pair.first;
    Class* current_class = classes_pair.second.get();

    if (current_class && current_class->get_user_playable() && options)
    {
      Option current_option;
      current_option.set_id(current_id);
      current_option.set_description(StringTable::get(current_class->get_class_name_sid()));

      options->add_option(current_option);
      options->add_option_description(StringTable::get(current_class->get_class_short_description_sid()));
      current_id++;
    }
  }

  // Random selection (use '*')
  Option random_option;
  random_option.set_id(random_option.get_id_for_random());
  random_option.set_description(StringTable::get(TextKeys::RANDOM_SELECTION));

  options->add_option(random_option);
  options->add_option_description("");

  vector<ScreenComponentPtr> cur_page;
  cur_page.push_back(make_shared<TextComponent>(synop.str()));
  cur_page.push_back(class_selection_text);
  cur_page.push_back(options);

  add_page(cur_page);

  // Set the prompt
  PromptPtr any_key_prompt = std::make_unique<Prompt>(PromptLocation::PROMPT_LOCATION_LOWER_RIGHT);
  any_key_prompt->set_text_sid(PromptTextKeys::PROMPT_SELECT_AN_OPTION);
  user_prompt = std::move(any_key_prompt);
}
