#include "stdafx.h"

#include "player.h"

#define POP_GROWTH_RATE .05
#define PRODUCTION_PER_POP 1.

#define FACTORIES_PER_POP 2
#define PRODUCTION_PER_FACTORY 10.
#define FACTORY_COST 10.

Player::Player() {}

double Player::getPopGrowthRate() {
  return POP_GROWTH_RATE;
}

double Player::getProductionPerPop() {
  return PRODUCTION_PER_POP;
}

uint32_t Player::getFactoriesPerPop() {
  return FACTORIES_PER_POP;
}

double Player::getFactoryCost() {
  return FACTORY_COST;
}

double Player::getProductionPerFactory() {
  return PRODUCTION_PER_FACTORY;
}
