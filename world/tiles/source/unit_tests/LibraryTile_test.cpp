#include "gtest/gtest.h"

TEST(SW_World_Tiles_LibraryTile, type_is_TILE_TYPE_LIBRARY)
{
  LibraryTile library_tile;

  EXPECT_EQ(TileType::TILE_TYPE_LIBRARY, library_tile.get_tile_type());
}

TEST(SW_World_Tiles_LibraryTile, tile_description_sid)
{
  LibraryTile library_tile;

  EXPECT_EQ(TileTextKeys::TILE_DESC_LIBRARY, library_tile.get_tile_description_sid());
}

TEST(SW_World_Tiles_LibraryTile, serialization_id)
{
  LibraryTile library_tile;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_LIBRARY_TILE, library_tile.get_class_identifier());
}

