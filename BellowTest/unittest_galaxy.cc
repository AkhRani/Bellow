#include "galaxy.h"
#include "testutils.h"
#include "mock_game.h"
#include "gtest/gtest.h"
#include "star_system.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;

Galaxy CreateTwoSystems(IGame& game, lua_State *L) {
  // Nominal, unowned planet
  RunLua(L, "none = { amount = 0, invested = 0 };"
    "unowned = { name = \"Sol\", pos = {x = 1.5, y = 1.5}, "
    "planet = { name = \"earth\", base_population = 100, population = none, factories = none } };"
    "empty = { name = \"Alpha Centurai\", pos = { x = 2.9, y = 3.5} };"
    "return { size = 5, systems = { empty, unowned } };");
  Galaxy galaxy(game, L);
  galaxy.FinishLoad();
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(2, galaxy.GetSystemCount());
  return galaxy;
}

TEST(GalaxyTest, LoadAndSave) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, empty system
  RunLua(L, "systems = { { name = \"Sol\", pos = { x = .5, y = .5} } }; return { size = 5, systems = systems }");
  Galaxy noplanet(game, L);
  noplanet.FinishLoad();
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(1, noplanet.GetSystemCount());

  Galaxy g2 = CreateTwoSystems(game, L);

  string rep = "return { gal = ";
  g2.Save(rep);
  rep.append(", other = 123}");
  RunLua(L, rep.c_str());
  EXPECT_EQ(1, lua_gettop(L));
  Galaxy g3(game, L, "gal");
  g3.FinishLoad();
  EXPECT_EQ(2, g3.GetSystemCount());

  // Make sure we cleaned up after ourselves
  EXPECT_EQ(1, lua_gettop(L));
  lua_getfield(L, -1, "other");
  EXPECT_EQ(123, lua_tointeger(L, -1));
  lua_pop(L, 2);
}

TEST(GalaxyTest, SystemAccessors) {
  MockGame game;
  lua_State *L = luaL_newstate();
  Galaxy galaxy(CreateTwoSystems(game, L));
  EXPECT_EQ(2, galaxy.GetSystemCount());

  StarSystem *pAlphaCenturai = galaxy.GetStarSystem(0);
  EXPECT_EQ("Alpha Centurai", pAlphaCenturai->m_Name);
  EXPECT_EQ(0, pAlphaCenturai->m_ID);

  StarSystem *pSol = galaxy.GetStarSystem(1);
  EXPECT_EQ("Sol", pSol->m_Name);

  EXPECT_EQ(nullptr, galaxy.GetStarSystem(-1));
  EXPECT_EQ(nullptr, galaxy.GetStarSystem(3));

  int count = 0;
  galaxy.VisitSystems(Galaxy::SystemVisitor([&count] (StarSystem& system) { count++; }));
  EXPECT_EQ(2, count);
}