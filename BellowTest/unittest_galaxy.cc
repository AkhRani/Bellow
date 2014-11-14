#include "galaxy.h"
#include "gtest/gtest.h"

TEST(GalaxyTest, Creation) {
  Galaxy g;
  EXPECT_EQ(0, g.SystemCount());
  EXPECT_EQ(g.BeginSystems(), g.EndSystems());
}
