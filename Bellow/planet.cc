#include <math.h>
#include <algorithm>

#include "planet.h"
#include "player.h"
extern "C" {
#include "lua.h"
}

#define POP_GROWTH_MODIFIER 1.

Planet::Planet(uint32_t maxPop) :
  m_pOwner(nullptr),
  m_population(0., 0., 0, maxPop),
  m_factories(0., 0., 0, maxPop)
{
}

/** Create a planet from the table on top of the Lua stack.
 * structure: { name = "earth", population = { cur = 10, max = 100 } }
 */
Planet *Planet::Load(lua_State *L) {
  lua_getfield(L, -1, "population");
  lua_getfield(L, -1, "max");
  int maxpop = lua_tointeger(L, -1);
  lua_pop(L, 2);
  return new Planet(maxpop);
}

void Planet::Colonize(Player *owner, uint32_t pop) {
  m_pOwner = owner;
  m_population.SetAmount(pop);
  m_population.SetGrowthRate(owner->GetPopGrowthRate());
  // Existing factories (if any) remain
  m_factories.SetCost(owner->GetFactoryCost());
  m_factories.SetMax(m_population.GetMax() * owner->GetFactoriesPerPop());
}

void Planet::Update() {
  if (!m_pOwner) {
    return;
  }
  uint32_t pop = m_population.GetAmount();
  uint32_t activeFactories =
    std::min(m_factories.GetAmount(), pop * m_pOwner->GetFactoriesPerPop());

  double capital = pop * m_pOwner->GetProductionPerPop();
  capital += activeFactories * m_pOwner->GetProductionPerFactory();

  m_population.Grow(0.);
  m_factories.Grow(capital);
}

