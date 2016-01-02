#include "SkillManager.hpp"
#include "StatusEffectFactory.hpp"
#include "ToHitCalculator.hpp"
#include "WeaponManager.hpp"

using namespace std;

const int ToHitCalculator::NWP_SKILL_BONUS_DIVISOR = 5;

ToHitCalculator::ToHitCalculator()
: attack_type(AttackType::ATTACK_TYPE_MELEE_PRIMARY)
{
}

ToHitCalculator::ToHitCalculator(const AttackType new_attack_type)
: attack_type(new_attack_type)
{
}

// The level portion of the to-hit bonus is equal to half the creature's
// level, rounded down.
int ToHitCalculator::get_level_bonus(CreaturePtr creature)
{
  int level_bonus = 0;

  if (creature != nullptr)
  {
    level_bonus = creature->get_level().get_current() / 2;
  }

  return level_bonus;
}

int ToHitCalculator::get_weapon_bonus(CreaturePtr creature)
{
  int weapon_bonus = 0;
  
  WeaponManager wm;
  WeaponPtr weapon = wm.get_weapon(creature, attack_type);
  
  SkillManager sm;

  // Apply the weapon modifier.
  SkillType st = SkillType::SKILL_MELEE_UNARMED;
  
  if (attack_type == AttackType::ATTACK_TYPE_RANGED && weapon)
  {
    st = weapon->get_trained_ranged_skill();
  }
  else
  {
    if (weapon)
    {
      st = weapon->get_trained_skill();
    }
  }

  weapon_bonus += (sm.get_skill_value(creature, st) / 2);  
  return weapon_bonus;
}

int ToHitCalculator::get_modifier_bonus(CreaturePtr creature)
{
  int to_hit_bonus = 0;

  if (creature != nullptr)
  {
    const map<double, vector<pair<string, Modifier>>>& modifiers = creature->get_modifiers_ref();

    for (const auto& mod_pair : modifiers)
    {
      for (const auto& current_mod_pair : mod_pair.second)
      {
        to_hit_bonus += current_mod_pair.second.get_to_hit_modifier();
      }
    }
  }

  return to_hit_bonus;
}

