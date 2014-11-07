#include "stdafx.h"

#include <math.h>
#include <algorithm>

#include "planet.h"
#include "player.h"

#define POP_GROWTH_MODIFIER 1.

Planet::Planet(uint32_t maxPop) :
  m_pOwner(nullptr),
  m_population(0., 0., 0, maxPop),
  m_factories(0., 0., 0, maxPop)
{
}

void Planet::Colonize(Player *owner, uint32_t pop) {
  m_pOwner = owner;
  m_population.setAmount(pop);
  m_population.setGrowthRate(owner->getPopGrowthRate());
  // Existing factories (if any) remain
  m_factories.setCost(owner->getFactoryCost());
  m_factories.setMax(m_population.getMax() * owner->getFactoriesPerPop());
}

void Planet::Update() {
  if (!m_pOwner) {
    return;
  }
  uint32_t pop = m_population.getAmount();
  uint32_t activeFactories =
    std::min(m_factories.getAmount(), pop * m_pOwner->getFactoriesPerPop());

  double capital = pop * m_pOwner->getProductionPerPop();
  capital += activeFactories * m_pOwner->getProductionPerFactory();

  m_population.grow(0.);
  m_factories.grow(capital);
}

