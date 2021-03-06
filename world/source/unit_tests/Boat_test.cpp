#include "gtest/gtest.h"

TEST(SW_World_Boat, serialization_id)
{
  Boat boat;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_BOAT, boat.get_class_identifier());
}

TEST(SW_World_Boat, saveload)
{
  Boat boat, boat2;

  // Not the greatest boat, really.
  boat.set_material_type(MaterialType::MATERIAL_TYPE_STONE);

  ostringstream ss;

  boat.serialize(ss);

  istringstream iss(ss.str());

  boat2.deserialize(iss);

  EXPECT_TRUE(boat == boat2);
}
