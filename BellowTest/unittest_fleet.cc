#include "game.h"
#include "fleet.h"
#include "player.h"
#include "system_info.h"

#include "gtest/gtest.h"
#include "testutils.h"
#include "mock_galaxy.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;

TEST(FleetTest, LoadSave) {
  MockGalaxy galaxy;
  lua_State *L = luaL_newstate();

  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(galaxy, L);

  SystemInfo info;
  Fleet fleet(p1, 3., 4.);
  EXPECT_EQ(true, fleet.InOrbit());

  string rep("return ");
  fleet.Save(rep);
  RunLua(L, rep.c_str());

  Fleet f2(p1, L);
  EXPECT_EQ(0, lua_gettop(L));
  double x, y;
  f2.GetPosition(x, y);
  EXPECT_EQ(3., x);
  EXPECT_EQ(4., y);
}


TEST(FleetTest, Move) {
  struct MoveScenario {
    double initX, initY;
    double destX, destY;
    int turns;
  };

  MoveScenario scenarios[] = {
    { 0., 0., 2., 0., 2 },
    { 0., 0., -2., 0., 2 },
    { 0., 0., 0., 2., 2 },
    { 0., 0., 0., -2., 2 },
    { 0., 0., 2., 2., 3 },
    { 0., 0., 3., 4., 5 },
  };

  MockGame game;
  MockGalaxy galaxy;
  for (auto &scenario : scenarios) {
    galaxy.AddStarSystem(game, scenario.destX, scenario.destY);
  }

  lua_State *L = luaL_newstate();
  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(galaxy, L);

  int destId = 1;
  for (auto& scenario : scenarios) {
    double destX = scenario.destX;
    double destY = scenario.destY;
    double initX = scenario.initX;
    double initY = scenario.initY;
    double x, y;

    // TODO:  Create fleet with system ID
    Fleet fleet(p1, initX, initY);
    EXPECT_EQ(true, fleet.InOrbit());
    fleet.SetDestination(destX, destY);
    for (int i = 0; i < scenario.turns - 1; i++) {
      fleet.Move();
      fleet.GetPosition(x, y);

      // Make sure movement is in the correct direction
      if (destX < initX) {
        EXPECT_LT(x, initX);
        EXPECT_LT(destX, x);
      }
      else if (initX < destX) {
        EXPECT_LT(initX, x);
        EXPECT_LT(x, destX);
      }
      else {
        EXPECT_NEAR(destX, x, .000001);
      }

      if (destY < initY) {
        EXPECT_LT(y, initY);
        EXPECT_LT(destY, y);
      }
      else if (initY < destY) {
        EXPECT_LT(initY, y);
        EXPECT_LT(y, destY);
      }
      else {
        EXPECT_NEAR(destY, y, .000001);
      }

      EXPECT_EQ(false, fleet.InOrbit());
    }

    fleet.Move();
    fleet.GetPosition(x, y);
    EXPECT_EQ(destX, x);
    EXPECT_EQ(destY, y);
    EXPECT_EQ(true, fleet.InOrbit());
  }

}