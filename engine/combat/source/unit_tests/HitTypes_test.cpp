#include "gtest/gtest.h"

TEST(SW_Engine_Combat_HitTypes, is_critical_hit)
{
  int i = CombatConstants::CRITICAL_DIFFICULTY;
  
  EXPECT_EQ(HitTypeEnum::HIT_TYPE_CRITICAL, HitTypeEnumConverter::from_successful_to_hit_roll(i));
  
  i--;
  
  EXPECT_FALSE(HitTypeEnum::HIT_TYPE_CRITICAL == HitTypeEnumConverter::from_successful_to_hit_roll(i));
  
  i -= 15;
  
  EXPECT_FALSE(HitTypeEnum::HIT_TYPE_CRITICAL == HitTypeEnumConverter::from_successful_to_hit_roll(i));
}

TEST(SW_Engine_Combat_HitTypes, is_mighty_blow)
{
  for (int i = CombatConstants::MIGHTY_BLOW_DIFFICULTY; i < CombatConstants::CRITICAL_DIFFICULTY; i++)
  {
    EXPECT_TRUE(HitTypeEnum::HIT_TYPE_MIGHTY == HitTypeEnumConverter::from_successful_to_hit_roll(i));
  }
}

