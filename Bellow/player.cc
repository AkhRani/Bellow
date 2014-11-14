#include "player.h"

#define POP_GROWTH_RATE .05
#define PRODUCTION_PER_POP 1.

#define FACTORIES_PER_POP 2
#define PRODUCTION_PER_FACTORY 10.
#define FACTORY_COST 10.

Player::Player() {}

double Player::GetPopGrowthRate() {
  return POP_GROWTH_RATE;
}

double Player::GetProductionPerPop() {
  return PRODUCTION_PER_POP;
}

uint32_t Player::GetFactoriesPerPop() {
  return FACTORIES_PER_POP;
}

double Player::GetFactoryCost() {
  return FACTORY_COST;
}

double Player::GetProductionPerFactory() {
  return PRODUCTION_PER_FACTORY;
}
