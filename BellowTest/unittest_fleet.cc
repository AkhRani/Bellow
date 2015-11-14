#include "game.h"
#include "fleet.h"
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

using std::string;

TEST(FleetTest, LoadSave) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Create system for fleets to orbit
  game.m_Galaxy.AddStarSystem(game, 3., 4.);

  // Create player to own fleets
  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(game.m_Galaxy, L);

  // Test Save
  SystemInfo info;
  Fleet fleet(p1, game.m_Galaxy, 0);
  EXPECT_EQ(true, fleet.IsInOrbit());
  EXPECT_EQ(false, fleet.IsLaunching());

  string rep("return ");
  fleet.Save(rep);
  RunLua(L, rep.c_str());

  Fleet f2(p1, game.m_Galaxy, L);
  EXPECT_EQ(0, lua_gettop(L));
  double x, y;
  f2.GetPosition(x, y);
  EXPECT_EQ(3., x);
  EXPECT_EQ(4., y);

  // Test off-nominal loads
  // Shouldn't be orbiting and have a remote destination
  RunLua(L, "return { x=0, y=0, st=0, target = 1 }");
  EXPECT_THROW(Fleet bad1(p1, game.m_Galaxy, L), std::runtime_error);
}


TEST(FleetTest, Move) {
  const double EPSILON(.000001);

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
  game.m_Galaxy.AddStarSystem(game, 0., 0.);
  for (auto &scenario : scenarios) {
    game.m_Galaxy.AddStarSystem(game, scenario.destX, scenario.destY);
  }

  lua_State *L = luaL_newstate();
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(game.m_Galaxy, L);

  int startId = 0;
  int destId = 1;
  for (auto& scenario : scenarios) {
    double destX = scenario.destX;
    double destY = scenario.destY;
    double initX = scenario.initX;
    double initY = scenario.initY;
    double x, y;

    Fleet fleet(p1, game.m_Galaxy, startId);
    EXPECT_EQ(true, fleet.IsInOrbit());
    EXPECT_EQ(false, fleet.IsLaunching());

    // Go over there!
    fleet.SetDestination(destId);
    EXPECT_EQ(true, fleet.IsLaunching());
    EXPECT_EQ(false, fleet.IsInOrbit());

    // Changed my mind
    fleet.SetDestination(startId);
    EXPECT_EQ(true, fleet.IsInOrbit());
    EXPECT_EQ(false, fleet.IsLaunching());

    // Changed my mind again
    fleet.SetDestination(destId);
    EXPECT_EQ(true, fleet.IsLaunching());
    EXPECT_EQ(false, fleet.IsInOrbit());

    for (int i = 0; i < scenario.turns - 1; i++) {
      string rep("return ");
      fleet.Save(rep);
      RunLua(L, rep.c_str());
      Fleet reloaded(p1, game.m_Galaxy, L);
      if (0 == i) {
        EXPECT_EQ(true, reloaded.IsLaunching());
      }

      reloaded.Move();
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
        EXPECT_NEAR(destX, x, EPSILON);
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
        EXPECT_NEAR(destY, y, EPSILON);
      }

      double rx, ry;
      reloaded.GetPosition(rx, ry);
      EXPECT_NEAR(x, rx, EPSILON);
      EXPECT_NEAR(y, ry, EPSILON);

      EXPECT_EQ(false, fleet.IsInOrbit());
    }

    fleet.Move();
    fleet.GetPosition(x, y);
    EXPECT_EQ(destX, x);
    EXPECT_EQ(destY, y);
    EXPECT_EQ(false, fleet.IsInOrbit());
    EXPECT_EQ(true, fleet.IsArriving());

    // Arrival happens in stages
    fleet.Arrive();
    EXPECT_EQ(false, fleet.IsArriving());
    EXPECT_EQ(true, fleet.IsInOrbit());
    destId++;
  }
}

TEST(FleetTest, Exploration) {
  MockGame game;
  game.m_Galaxy.AddStarSystem(game, 0., 0.);
  game.m_Galaxy.AddStarSystem(game, .5, .5);

  lua_State *L = luaL_newstate();
  RunLua(L, "return { name = \"Kirk\", race = \"Human\" }");
  Player p1(game.m_Galaxy, L);
  p1.SetSystemCount(2);

  PlayerSystemInfo unexplored { "?", 0, 0 };
  p1.SetSystemInfo(1, unexplored );
  SystemInfo unex;
  p1.GetSystemInfo(1, unex);
  EXPECT_EQ("?", unex.name);

  Fleet fleet(p1, game.m_Galaxy, 0);
  fleet.SetDestination(1);
  fleet.Move();
  fleet.Arrive();
  SystemInfo explored;
  p1.GetSystemInfo(1, explored);
  EXPECT_EQ("dummy", explored.name);
}

// This class tests interactions between the Fleet and Planet classes
TEST(FleetTest, Transports) {

}