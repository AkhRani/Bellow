#include "galaxy.h"
#include "testutils.h"
#include "gtest/gtest.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;

TEST(GalaxyTest, LoadAndSave) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, empty system
  RunLua(L, "systems = { { name = \"Sol\", x = .5, y = .5} }; return { size = 5, systems = systems }");
  Galaxy noplanet(game, L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(1, noplanet.SystemCount());

  // Nominal, unowned planet
  RunLua(L, "none = { amount = 0, invested = 0 };"
    "unowned = { name = \"Sol\", x = 1.5, y = 1.5, "
      "planet = { name = \"earth\", base_population = 100, population = none, factories = none } };"
    "empty = { name = \"Alpha Centurai\", x = 2.9, y = 3.5 };"
    "return { size = 5, systems = { empty, unowned } };");
  Galaxy g2(game, L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(2, g2.SystemCount());

  string serialized = "return ";
  g2.Save(serialized);
  RunLua(L, serialized.c_str());
  Galaxy g3(game, L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(2, g3.SystemCount());
}
