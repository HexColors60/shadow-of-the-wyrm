#include "gtest/gtest.h"

TEST(SL_Engine_Time_WorldTimeKeeper, serialization_id)
{
  WorldTimeKeeper wtk;

  EXPECT_EQ(CLASS_ID_WORLD_TIME_KEEPER, wtk.get_class_identifier());
}

