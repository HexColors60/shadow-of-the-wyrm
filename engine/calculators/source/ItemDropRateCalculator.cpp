#include "ItemDropRateCalculator.hpp"
#include "CreatureGenerationConstants.hpp"

// Dungeoneering acts as a kind of item-luck, allowing the creature to find
// more items.
int ItemDropRateCalculator::calculate_pct_chance_item_drop(CreaturePtr creature)
{
  int pct_chance = CreatureGenerationConstants::CREATURE_DROP_RATE;
  return calculate_pct_chance_item_drop(creature, pct_chance);
}

int ItemDropRateCalculator::calculate_pct_chance_item_drop(CreaturePtr creature, const int base_pct_chance)
{
  int pct_chance = base_pct_chance;

  if (creature != nullptr)
  {
    float multiplier = 1.0f + static_cast<float>(creature->get_skills().get_value(SkillType::SKILL_GENERAL_DUNGEONEERING) / 100.0f);
    pct_chance = static_cast<int>(pct_chance * multiplier);
  }

  return pct_chance;
}

#ifdef UNIT_TESTS
#include "unit_tests/ItemDropRateCalculator_test.cpp"
#endif