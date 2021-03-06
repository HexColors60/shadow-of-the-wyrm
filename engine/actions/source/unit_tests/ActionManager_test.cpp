#include "gtest/gtest.h"

TEST(SW_Engine_Actions_ActionManager, serialization_id)
{
  ActionManager am;

  EXPECT_EQ(ClassIdentifier::CLASS_ID_ACTION_MANAGER, am.get_class_identifier());
}

TEST(SW_Engine_Actions_ActionManager, saveload)
{
  ActionManager am, am2;
  ostringstream ss;

  am.serialize(ss);

  istringstream iss(ss.str());

  am2.deserialize(iss);

  EXPECT_TRUE(am == am2);
}