#include "gtest/gtest.h"
#include "testutils.h"

#include "star_system.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::runtime_error;

TEST(StarSystemTest, Load) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, no planet
  RunLua(L, "return { name = \"Sol\", x = .5, y = .5}");
  StarSystem noplanet(game, L);

  // Nominal, unowned planet
  RunLua(L, "none = { amount = 0, invested = 0 } return { name = \"Sol\", x = .5, y = .5, "
    "planet = { name = \"earth\", base_population = 100, population = none, factories = none } }");
  StarSystem withplanet(game, L);
}
