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
using std::weak_ptr;

TEST(StarSystemTest, LoadAndSave) {
  MockGame game;
  lua_State *L = luaL_newstate();

  // Nominal, no planet
  RunLua(L, "return { name = \"Sol\", x = .5, y = .6}");
  StarSystem noplanet(game, L, 1);
  EXPECT_EQ("Sol", noplanet.m_Name);
  EXPECT_EQ(.5, noplanet.m_X);
  EXPECT_EQ(.6, noplanet.m_Y);
  EXPECT_EQ(1, noplanet.m_ID);
  weak_ptr<Planet> p = noplanet.GetPlanet();
  EXPECT_EQ(p.lock(), nullptr);

  // Nominal, unowned planet
  RunLua(L, "none = { amount = 0, invested = 0 } return { name = \"Betel\", x = .6, y = .7, "
    "planet = { name = \"earth\", base_population = 100, population = none, factories = none } }");
  StarSystem withplanet(game, L, 2);
  EXPECT_EQ("Betel", withplanet.m_Name);
  EXPECT_EQ(.6, withplanet.m_X);
  EXPECT_EQ(.7, withplanet.m_Y);
  EXPECT_EQ(2, withplanet.m_ID);
  weak_ptr<Planet> wp = withplanet.GetPlanet();
  auto planet(wp.lock());
  EXPECT_NE(nullptr, planet);
  EXPECT_EQ(100, planet->GetMaxPopulation());


  string serialized = "return ";
  withplanet.Save(serialized);
  RunLua(L, serialized.c_str());
  StarSystem loaded(game, L, 1);
  EXPECT_EQ("Betel", loaded.m_Name);
  EXPECT_EQ(.6, loaded.m_X);
  EXPECT_EQ(.7, loaded.m_Y);
  EXPECT_EQ(1, loaded.m_ID);
  weak_ptr<Planet> lwp = loaded.GetPlanet();
  auto loadedPlanet(lwp.lock());
  EXPECT_NE(nullptr, loadedPlanet);
  EXPECT_EQ(100, loadedPlanet->GetMaxPopulation());
}
