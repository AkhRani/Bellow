extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "planet.h"
#include "player.h"
#include "gtest/gtest.h"

// This test is named "Creation", and belongs to the "PlanetTest" test case.
TEST(PlanetTest, Creation) {
  Planet p(100);
  EXPECT_EQ(0, p.GetPopulation());
  EXPECT_EQ(0, p.GetFactories());
  EXPECT_EQ(100, p.GetMaxPopulation());
}

TEST(PlanetTest, Load) {
  lua_State *L = luaL_newstate();
  EXPECT_EQ(false, luaL_dostring(L, "return { name = \"earth\", base_population = 100, population = { amount = 10, fractional = 0. } }"));
  EXPECT_EQ(true, lua_istable(L, -1));
  Planet *p = Planet::Load(L);
  EXPECT_NE(nullptr, p);
  EXPECT_EQ(100, p->GetMaxPopulation());
  EXPECT_EQ(10, p->GetPopulation());

  // TODO:  Test Error Handling
}

TEST(PlanetTest, Save) {
  Planet p(100);
  std::string serialized = "return ";
  p.Save(serialized);

  lua_State *L = luaL_newstate();
  EXPECT_EQ(false, luaL_dostring(L, serialized.c_str()));
  EXPECT_EQ(true, lua_istable(L, -1));
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
