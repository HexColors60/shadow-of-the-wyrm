#include "gtest/gtest.h"

TEST(SW_World_Tiles_DungeonTile, type_is_TILE_TYPE_DUNGEON)
{
  DungeonTile dungeon_tile;

  EXPECT_EQ(TileType::TILE_TYPE_DUNGEON, dungeon_tile.get_tile_type());
}

TEST(SW_World_Tiles_DungeonTile, serialization_id)
{
  DungeonTile dungeon_tile;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_DUNGEON_TILE, dungeon_tile.get_class_identifier());
}

