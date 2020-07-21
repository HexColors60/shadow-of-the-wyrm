#include "gtest/gtest.h"

TEST(SW_Engine_Calculators_PacificationCalculator, calculate_pct_chance_pacify_music)
{
  CreaturePtr music_creature;
  CreaturePtr fov_creature;
  PacificationCalculator pc;
  ItemStatus status = ItemStatus::ITEM_STATUS_UNCURSED;

  EXPECT_EQ(0, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, status));
  EXPECT_EQ(0, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, status));

  music_creature = std::make_shared<Creature>();
  fov_creature = std::make_shared<Creature>();

  music_creature->get_skills().set_value(SkillType::SKILL_GENERAL_MUSIC, 30);
  music_creature->set_level(10);
  fov_creature->set_level(5);

  music_creature->set_charisma(22);
  fov_creature->set_willpower(10);

  EXPECT_EQ(39, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, ItemStatus::ITEM_STATUS_UNCURSED));
  EXPECT_EQ(64, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, ItemStatus::ITEM_STATUS_UNCURSED));

  EXPECT_EQ(19, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, ItemStatus::ITEM_STATUS_CURSED));
  EXPECT_EQ(44, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, ItemStatus::ITEM_STATUS_CURSED));

  EXPECT_EQ(59, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, ItemStatus::ITEM_STATUS_BLESSED));
  EXPECT_EQ(80, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, ItemStatus::ITEM_STATUS_BLESSED));

  fov_creature->get_skills().set_value(SkillType::SKILL_GENERAL_MUSIC, 10);

  EXPECT_EQ(29, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, ItemStatus::ITEM_STATUS_UNCURSED));
  EXPECT_EQ(54, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, ItemStatus::ITEM_STATUS_UNCURSED));

  music_creature->set_level(200);

  EXPECT_EQ(80, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, false, status));
  EXPECT_EQ(80, pc.calculate_pct_chance_pacify_music(music_creature, fov_creature, true, status));
}

TEST(SW_Engine_Calculators_PacificationCalculator, calculate_pct_chance_tame)
{
  PacificationCalculator pc;

  EXPECT_EQ(0, pc.calculate_pct_chance_tame_beastmastery(nullptr, nullptr));

  CreaturePtr creature = std::make_shared<Creature>();
  CreaturePtr to_tame = std::make_shared<Creature>();

  EXPECT_EQ(0, pc.calculate_pct_chance_tame_beastmastery(creature, nullptr));
  EXPECT_EQ(0, pc.calculate_pct_chance_tame_beastmastery(nullptr, to_tame));

  std::map<int, int> exp_vals = { {0,0}, {10,0}, {15,5}, {26,16}, {37,27}, {43,33}, {54,44}, {65,55},
                                  {77,67}, {88,78}, {99,80}, {100, 80} };

  for (auto v : exp_vals)
  {
    creature->get_skills().set_value(SkillType::SKILL_GENERAL_BEASTMASTERY, v.first);
    EXPECT_EQ(v.second, pc.calculate_pct_chance_tame_beastmastery(creature, to_tame));
  }

  creature->get_skills().set_value(SkillType::SKILL_GENERAL_BEASTMASTERY, 50);
  creature->set_level(11);
  to_tame->set_level(1);

  EXPECT_EQ(50, pc.calculate_pct_chance_tame_beastmastery(creature, to_tame));

  creature->set_level(1);
  to_tame->set_level(11);

  EXPECT_EQ(30, pc.calculate_pct_chance_tame_beastmastery(creature, to_tame));
}

TEST(SW_Engine_Calculators_PacificationCalculator, calculate_exp_proportion)
{
  PacificationCalculator pc;
  CreaturePtr creature = std::make_shared<Creature>();

  EXPECT_DOUBLE_EQ(0.0, pc.calculate_exp_proportion(nullptr, SkillType::SKILL_GENERAL_BEASTMASTERY));
  EXPECT_DOUBLE_EQ(0.0, pc.calculate_exp_proportion(creature, SkillType::SKILL_GENERAL_BEASTMASTERY));

  std::vector<SkillType> pacification_skills = {SkillType::SKILL_GENERAL_BEASTMASTERY, SkillType::SKILL_GENERAL_MUSIC};

  for (auto skill : pacification_skills)
  {
    for (int i = 1; i <= 100; i++)
    {
      double exp_val = static_cast<double>(i) / 100.0;
      creature->get_skills().set_value(skill, i);

      EXPECT_DOUBLE_EQ(exp_val, pc.calculate_exp_proportion(creature, skill));
    }
  }
}