#include "gtest/gtest.h"

TEST(SW_World_MovementAccumulation, serialization_id)
{
  MovementAccumulation ma;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_MOVEMENT_ACCUMULATION, ma.get_class_identifier());
}

TEST(SW_World_MovementAccumulation, saveload)
{
  MovementAccumulation ma, ma2;

  ma.set_movement_type(MovementType::MOVEMENT_TYPE_BOAT);
  ma.set_minutes_on_super_type_given_movement(123);
  ma.set_tile_super_type(TileSuperType::TILE_SUPER_TYPE_WATER);
  ma.set_minutes_on_tile_type_given_movement(321);
  ma.set_tile_type(TileType::TILE_TYPE_SEA);

  ostringstream ss;

  ma.serialize(ss);

  istringstream iss(ss.str());

  ma2.deserialize(iss);

  EXPECT_TRUE(ma == ma2);
}
