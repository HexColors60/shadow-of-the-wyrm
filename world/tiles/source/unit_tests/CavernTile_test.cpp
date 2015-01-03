#include "gtest/gtest.h"

TEST(SL_World_Tiles_CavernTile, type_is_TILE_TYPE_CAVERN)
{
  CavernTile cavern_tile;

  EXPECT_EQ(TILE_TYPE_CAVERN, cavern_tile.get_tile_type());
}

TEST(SL_World_Tiles_CavernTile, serialization_id)
{
  CavernTile cavern_tile;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_CAVERN_TILE, cavern_tile.get_class_identifier());
}

