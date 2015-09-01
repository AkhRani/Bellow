#include "galaxy.h"
#include "testutils.h"
#include "gtest/gtest.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

TEST(GalaxyTest, Creation) {
  Galaxy g;
  EXPECT_EQ(0, g.SystemCount());
  EXPECT_EQ(g.BeginSystems(), g.EndSystems());
}

TEST(GalaxyTest, Load) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, empty system
  RunLua(L, "systems = { { name = \"Sol\", x = .5, y = .5} }; return systems");
  Galaxy noplanet(game, L);
  EXPECT_EQ(1, noplanet.SystemCount());

  // Nominal, unowned planet
  RunLua(L, "none = { amount = 0, invested = 0 };"
    "unowned = { name = \"Sol\", x = .5, y = .5, "
      "planet = { name = \"earth\", base_population = 100, population = none, factories = none } };"
    "empty = { name = \"Alpha Centurai\", x = .9, y = .5 };"
    "return { empty, unowned };");
  Galaxy g2(game, L);
  EXPECT_EQ(2, g2.SystemCount());
}
