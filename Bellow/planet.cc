#include <math.h>
#include <algorithm>

#include "planet.h"
#include "player.h"
#include "util.h"
#include "game.h"

extern "C" {
#include "lua.h"
}

using std::string;
using std::shared_ptr;
using std::weak_ptr;

#define NOMINAL_POP_GROWTH 10   //!< 10% per turn

Planet::Planet(uint32_t maxPop) :
  m_basePop(maxPop),
  m_owner(),
  m_population(0, maxPop),
  m_factories(0, maxPop)
{
}

/** Create a planet from the table on top of the Lua stack.
* structure: { name = "earth", base_population = 100, population = { amount = 10, invested = 5 } }
*/
Planet::Planet(const Game &game, lua_State *L) {
  LoadCheck(lua_istable(L, -1));

  m_basePop = LoadCheckInteger(L, "base_population");

  lua_getfield(L, -1, "population");
  m_population.Load(L);

  lua_getfield(L, -1, "factories");
  m_factories.Load(L);

  m_population.SetMax(m_basePop);

  lua_getfield(L, -1, "owner");
  size_t len;
  const char *ownerName = lua_tolstring(L, -1, &len);
  if (ownerName) {
    m_owner = game.GetPlayer(ownerName);
    SetProductProperties();
  }
  lua_pop(L, 1);

  lua_pop(L, 1);
}

  
/** Create a planet from the table on top of the Lua stack.
 * structure: { name = "earth", base_population = 100, population = { amount = 10, invested = 5 } }
 */
Planet *Planet::Load(const Game &game, lua_State *L) {
  return new Planet(game, L);
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
  m_population.SetAmount(pop);
  SetProductProperties();
}

void Planet::SetProductProperties() {
  shared_ptr<Player> owner(m_owner);
  // Existing factories (if any) remain
  m_factories.SetMax(m_population.GetMax() * owner->GetFactoriesPerPop());
}

/**
 * See http://realmsbeyond.net/forums/archive/index.php?thread-1905.html for lots of good info
 */
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

  m_population.SetGrowthRate(NOMINAL_POP_GROWTH);
  m_population.Grow(owner->GetPopCost(), 0);
  m_factories.Grow(owner->GetFactoryCost(), uint32_t(floor(capital)));
}

