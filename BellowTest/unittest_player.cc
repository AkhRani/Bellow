#include "game.h"
#include "planet.h"
#include "player.h"
#include "system_info.h"

#include "gtest/gtest.h"
#include "testutils.h"
#include "mock_game.h"
#include "mock_galaxy.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

TEST(PlayerTest, LoadSave) {
  MockGame game;
  MockGalaxy galaxy;
  lua_State *L = luaL_newstate();

  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");

  Player p1(galaxy, L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(p1.GetName(), "Kirk");
  EXPECT_EQ(p1.GetFleetCount(), 0);

  RunLua(L, 
    "sysinfo = { { name = \"Sol\", x=.5, y=.6, fact=20, pop=30 } } "
    "return { name = \"Kirk\", race = \"Human\", fleets = { { x = 0, y = 0 } }, systems = sysinfo }");
  Player p2(galaxy, L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(p2.GetName(), "Kirk");
  EXPECT_EQ(p2.GetFleetCount(), 1);

  SystemInfo info;
  p2.GetSystemInfo(1, info);
  EXPECT_EQ(.5, info.x);
  EXPECT_EQ(.6, info.y);
  EXPECT_EQ(20, info.factories);
  EXPECT_EQ(30, info.population);
  EXPECT_EQ("Sol", info.name);

  string serial = "return ";
  p2.Save(serial);
  RunLua(L, serial.c_str());
  {
    Player p3(galaxy, L);

    EXPECT_EQ(0, lua_gettop(L));
    EXPECT_EQ(p3.GetName(), "Kirk");
    EXPECT_EQ(p3.GetFleetCount(), 1);

    SystemInfo info;
    p3.GetSystemInfo(1, info);
    EXPECT_EQ(.5, info.x);
    EXPECT_EQ(.6, info.y);
    EXPECT_EQ("Sol", info.name);
  }
}