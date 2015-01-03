#include "gtest/gtest.h"

TEST(SL_World_Tiles_UpStaircaseTile, type_is_TILE_TYPE_STAIRCASE)
{
  UpStaircaseTile ust;

  EXPECT_EQ(TileType::TILE_TYPE_UP_STAIRCASE, ust.get_tile_type());
}

TEST(SL_World_Tiles_UpStaircaseTile, staircase_type)
{
  UpStaircaseTile ust;

  EXPECT_EQ(STAIRCASE_UP, ust.get_staircase_type());
}

TEST(SL_World_Tiles_UpStaircaseTile, serialization_id)
{
  UpStaircaseTile ust;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_UP_STAIRCASE_TILE, ust.get_class_identifier());
}

