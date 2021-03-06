#include "gtest/gtest.h"

TEST(SW_World_Food, serialization_id)
{
  Food food;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_FOOD, food.get_class_identifier());
}

TEST(SW_World_Food, saveload)
{
  Food food, food2;

  food.set_material_type(MaterialType::MATERIAL_TYPE_PAPER); // Well...whatever.
  food.set_standard_drinks(3.1415f);
  food.set_food_type(FoodType::FOOD_TYPE_VEGETABLE);
  food.set_poisoned(true);

  ostringstream ss;

  food.serialize(ss);

  istringstream iss(ss.str());

  food2.deserialize(iss);

  EXPECT_TRUE(food == food2);
}
