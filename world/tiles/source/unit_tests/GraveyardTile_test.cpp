#include "gtest/gtest.h"

TEST(SW_World_Tiles_GraveyardTile, type_is_TILE_TYPE_GRAVEYARD)
{
  GraveyardTile graveyard_tile;

  EXPECT_EQ(TileType::TILE_TYPE_GRAVEYARD, graveyard_tile.get_tile_type());
}

TEST(SW_World_Tiles_GraveyardTile, tile_description_sid)
{
  GraveyardTile graveyard_tile;

  EXPECT_EQ(TileTextKeys::TILE_DESC_GRAVEYARD, graveyard_tile.get_tile_description_sid());
}

TEST(SW_World_Tiles_GraveyardTile, serialization_id)
{
  GraveyardTile graveyard_tile;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_GRAVEYARD_TILE, graveyard_tile.get_class_identifier());
}

