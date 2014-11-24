#include "planet.h"
#include "player.h"
#include "gtest/gtest.h"
#include "testutils.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// This test is named "Creation", and belongs to the "PlanetTest" test case.
TEST(PlanetTest, Creation) {
  Planet p(100);
  EXPECT_EQ(0, p.GetPopulation());
  EXPECT_EQ(0, p.GetFactories());
  EXPECT_EQ(100, p.GetMaxPopulation());
}

TEST(PlanetTest, Load) {
  const char *earth = "return { name = \"earth\", base_population = 100, population = { amount = 10, fractional = 0. } }";
  lua_State *L = luaL_newstate();

  RunLua(L, earth);
  Planet *p = Planet::Load(L);
  EXPECT_NE(nullptr, p);
  EXPECT_EQ(100, p->GetMaxPopulation());
  EXPECT_EQ(10, p->GetPopulation());

  RunLua(L, earth);
  Planet *pnew = new Planet(L);
  EXPECT_NE(nullptr, pnew);
  EXPECT_EQ(100, pnew->GetMaxPopulation());
  EXPECT_EQ(10, pnew->GetPopulation());

  // TODO:  Test Error Handling
}

TEST(PlanetTest, Save) {
  Planet p(100);
  std::string serialized = "return ";
  p.Save(serialized);

  lua_State *L = luaL_newstate();
  RunLua(L, serialized.c_str());
  Planet *restored = Planet::Load(L);
  EXPECT_NE(nullptr, restored);
  EXPECT_EQ(100, restored->GetMaxPopulation());
}

TEST(PlanetTest, Growth) {
  int i;
  Player owner;
  Planet planet(100);

  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(0, planet.GetPopulation()) << "No growth before colonization";

  planet.Colonize(&owner, 1);
  EXPECT_EQ(1, planet.GetPopulation());
  EXPECT_EQ(100, planet.GetMaxPopulation());

  // with 5% growth, 1.5 ** 14 == 0.9799
  for (i = 0; i < 14; i++) {
    planet.Update();
    EXPECT_EQ(1, planet.GetPopulation());
  }
  planet.Update();
  EXPECT_EQ(2, planet.GetPopulation()) << "Population should have increased";

  planet.Colonize(&owner, 100);
  EXPECT_EQ(100, planet.GetPopulation());
  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(100, planet.GetPopulation()) << "Population growth should respect max";
}
