#include <math.h>
#include <algorithm>

#include "planet.h"
#include "player.h"
#include "util.h"

extern "C" {
#include "lua.h"
}
using namespace std;

#define POP_GROWTH_MODIFIER 1.

Planet::Planet(uint32_t maxPop) :
  m_basePop(maxPop),
  m_owner(),
  m_population(0., 0., 0, maxPop),
  m_factories(0., 0., 0, maxPop)
{
}

/** Create a planet from the table on top of the Lua stack.
* structure: { name = "earth", base_population = 100, population = { amount = 10, fractional = .5 } }
*/
Planet::Planet(lua_State *L) {
  LoadCheck(lua_istable(L, -1));

  m_basePop = LoadCheckInteger(L, "base_population");

  lua_getfield(L, -1, "population");
  m_population.Load(L);

  // lua_getfield(L, -1, "factories");
  // m_factories.Load(L);

  m_population.SetMax(m_basePop);

  // TODO:  Recalculate max, cost, and growth rate based on owner
  lua_pop(L, 1);
}

  
/** Create a planet from the table on top of the Lua stack.
 * structure: { name = "earth", base_population = 100, population = { amount = 10, fractional = .5 } }
 */
Planet *Planet::Load(lua_State *L) {
  return new Planet(L);
}

void Planet::Save(string &serialized) {
  serialized.append("\n{ name = \"");
  serialized.append("dummy\"");
  serialized.append(", base_population = " + std::to_string(m_basePop));
  serialized.append(", population =");
  m_population.Save(serialized);
  serialized.append(", factories =");
  m_factories.Save(serialized);
  serialized.append(" }");
}

void Planet::Colonize(weak_ptr<Player> new_owner, uint32_t pop) {
  m_owner = new_owner;
  shared_ptr<Player> owner(m_owner);
  m_population.SetAmount(pop);
  m_population.SetGrowthRate(owner->GetPopGrowthRate());
  // Existing factories (if any) remain
  m_factories.SetCost(owner->GetFactoryCost());
  m_factories.SetMax(m_population.GetMax() * owner->GetFactoriesPerPop());
}

void Planet::Update() {
  if (m_owner.expired()) {
    // Either not colonized, or bug.  Don't think it's possible to tell which.
    return;
  }
  shared_ptr<Player> owner(m_owner);
  uint32_t pop = m_population.GetAmount();
  uint32_t activeFactories =
    std::min(m_factories.GetAmount(), pop * owner->GetFactoriesPerPop());

  double capital = pop * owner->GetProductionPerPop();
  capital += activeFactories * owner->GetProductionPerFactory();

  m_population.Grow(0.);
  m_factories.Grow(capital);
}

