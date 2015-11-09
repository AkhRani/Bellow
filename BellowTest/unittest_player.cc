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

  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(galaxy, L, 0);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(p1.GetName(), "Kirk");
  EXPECT_EQ(p1.GetFleetCount(), 0);
  EXPECT_EQ(p1.GetId(), 0);

  RunLua(L, 
    "sysinfo = { { name = \"Sol\", x=.5, y=.6, fact=20, pop=30 } } "
    "return { name = \"Kirk\", race = \"Human\", fleets = { { x = 0, y = 0 } }, systems = sysinfo }");
  Player p2(galaxy, L, 1);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(p2.GetName(), "Kirk");
  EXPECT_EQ(p2.GetFleetCount(), 1);
  EXPECT_EQ(p2.GetId(), 1);

  SystemInfo info;
  p2.GetSystemInfo(0, info);
  EXPECT_EQ(20, info.factories);
  EXPECT_EQ(30, info.population);
  EXPECT_EQ("Sol", info.name);

  string serial = "return ";
  p2.Save(serial);
  RunLua(L, serial.c_str());
  {
    Player p3(galaxy, L, 2);

    EXPECT_EQ(0, lua_gettop(L));
    EXPECT_EQ(p3.GetName(), "Kirk");
    EXPECT_EQ(p3.GetFleetCount(), 1);
    EXPECT_EQ(p3.GetId(), 2);

    SystemInfo info;
    p3.GetSystemInfo(0, info);
    EXPECT_EQ("Sol", info.name);
  }
}

TEST(PlayerTest, Colonize) {
  MockGame game;
  MockGalaxy galaxy;
  lua_State *L = luaL_newstate();

  galaxy.AddStarSystem(game, 0., 0.);
  EXPECT_EQ(nullptr, galaxy.GetStarSystem(0)->GetPlanet().GetOwner());

  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(galaxy, L, 0);
  p1.CreateFleet(0);
  EXPECT_EQ(p1.GetFleetCount(), 1);

  EXPECT_EQ(false, p1.Colonize(1));
  EXPECT_EQ(true, p1.Colonize(0));
  // Slight oddity caused by the mock game.
  EXPECT_EQ(game.GetPlayer(0), galaxy.GetStarSystem(0)->GetPlanet().GetOwner());
}
