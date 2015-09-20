#include "game.h"
#include "planet.h"
#include "player.h"
#include "gtest/gtest.h"
#include "testutils.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

TEST(PlayerTest, Load) {
  MockGame game;
  lua_State *L = luaL_newstate();

  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");

  Player p1(L);
  EXPECT_EQ(p1.GetName(), "Kirk");
  EXPECT_EQ(p1.GetFleetCount(), 0);

  RunLua(L, "return { name = \"Kirk\", race = \"Human\", fleets = { { x = 0, y = 0 } } }");
  Player p2(L);
  EXPECT_EQ(p2.GetName(), "Kirk");
  EXPECT_EQ(p2.GetFleetCount(), 1);
}