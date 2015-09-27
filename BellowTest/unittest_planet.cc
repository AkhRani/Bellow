#include <memory>

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

// This test is named "Creation", and belongs to the "PlanetTest" test case.
TEST(PlanetTest, Creation) {
  Planet p(100);
  EXPECT_EQ(0, p.GetPopulation());
  EXPECT_EQ(0, p.GetFactories());
  EXPECT_EQ(100, p.GetMaxPopulation());
}

TEST(PlanetTest, Load) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, unowned
  luaL_dostring(L, "empty = { amount = 0, invested = 0 }");
  RunLua(L, "return { name = \"earth\", base_population = 100, population = empty, factories = empty }");

  unique_ptr<Planet> p (Planet::Load(game, L));
  EXPECT_NE(nullptr, p);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(100, p->GetMaxPopulation());
  EXPECT_EQ(0, p->GetPopulation());
  EXPECT_EQ(0, p->GetFactories());

  // Nominal, owned
  luaL_dostring(L, "pop = { amount = 10, invested = 0 }");
  luaL_dostring(L, "fact = { amount = 20, invested = 0 }");
  RunLua(L, "return { name = \"earth\", owner = \"human\", base_population = 100, population = pop, factories = fact }");

  unique_ptr<Planet> pnew(new Planet(game, L));
  EXPECT_NE(nullptr, pnew);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(100, pnew->GetMaxPopulation());
  EXPECT_EQ(10, pnew->GetPopulation());
  EXPECT_EQ(200, pnew->GetMaxFactories());
  EXPECT_EQ(20, pnew->GetFactories());
  EXPECT_EQ("human", std::shared_ptr<Player>(pnew->GetOwner())->GetName());

  // TODO:  Test Error Handling
}

TEST(PlanetTest, Save) {
  MockGame game;
  Planet p(100);
  string serialized = "return ";
  p.Save(serialized);

  lua_State *L = luaL_newstate();
  RunLua(L, serialized.c_str());
  Planet *restored = Planet::Load(game, L);
  EXPECT_NE(nullptr, restored);
  EXPECT_EQ(100, restored->GetMaxPopulation());
}

TEST(PlanetTest, Growth) {
  int i;
  shared_ptr<Player> owner(new Player("human"));
  Planet planet(100);

  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(0, planet.GetPopulation()) << "No growth before colonization";

  planet.Colonize(owner, 1);
  EXPECT_EQ(1, planet.GetPopulation());
  EXPECT_EQ(100, planet.GetMaxPopulation());

  // Nominal 10% growth, not compounded
  for (i = 0; i < 9; i++) {
    planet.Update();
    EXPECT_EQ(1, planet.GetPopulation());
  }
  planet.Update();
  EXPECT_EQ(2, planet.GetPopulation()) << "Population should have increased";

  planet.Colonize(owner, 100);
  EXPECT_EQ(100, planet.GetPopulation());
  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(100, planet.GetPopulation()) << "Population growth should respect max";
}
