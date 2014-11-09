#include "planet.h"
#include "player.h"
#include "gtest/gtest.h"

// This test is named "Creation", and belongs to the "PlanetTest" test case.
TEST(PlanetTest, Creation) {
  Planet p(100);
  EXPECT_EQ(0, p.getPopulation());
  EXPECT_EQ(0, p.getFactories());
  EXPECT_EQ(100, p.getMaxPopulation());
}

TEST(PlanetTest, Growth) {
  int i;
  Player owner;
  Planet planet(100);

  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(0, planet.getPopulation()) << "No growth before colonization";

  planet.Colonize(&owner, 1);
  EXPECT_EQ(1, planet.getPopulation());
  EXPECT_EQ(100, planet.getMaxPopulation());

  // with 5% growth, 1.5 ** 14 == 0.9799
  for (i = 0; i < 14; i++) {
    planet.Update();
    EXPECT_EQ(1, planet.getPopulation());
  }
  planet.Update();
  EXPECT_EQ(2, planet.getPopulation()) << "Population should have increased";

  planet.Colonize(&owner, 100);
  EXPECT_EQ(100, planet.getPopulation());
  for (i = 0; i < 100; i++) {
    planet.Update();
  }
  EXPECT_EQ(100, planet.getPopulation()) << "Population growth should respect max";
}
