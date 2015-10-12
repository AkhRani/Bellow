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
using std::to_string;
using std::shared_ptr;
using std::weak_ptr;

#define NOMINAL_POP_GROWTH 10   //!< 10% per turn

Planet::Planet(IGame& game, uint32_t maxPop) :
  m_Game(game)
  , m_BasePop(maxPop)
  , m_OwnerId(0)
  , m_Population(0, maxPop)
  , m_Factories(0, maxPop)
{
}

/** Create a planet from the table on top of the Lua stack.
* structure: { name = "earth", base_population = 100, population = { amount = 10, invested = 5 } }
*/
Planet::Planet(IGame &game, lua_State *L) :
  m_Game(game) {
  LoadCheck(lua_istable(L, -1));

  m_BasePop = LoadCheckInteger(L, "base_population");

  lua_getfield(L, -1, "population");
  m_Population.Load(L);

  lua_getfield(L, -1, "factories");
  m_Factories.Load(L);

  m_Population.SetMax(m_BasePop);

  m_OwnerId = LoadOptInteger(L, "owner", 0);

  lua_pop(L, 1);
}

void Planet::Save(string &serialized) {
  serialized.append("\n{ name = \"");
  serialized.append("dummy\"");
  serialized.append(", base_population = " + to_string(m_BasePop));
  serialized.append(", population =");
  m_Population.Save(serialized);
  serialized.append(", factories =");
  m_Factories.Save(serialized);
  serialized.append(" }");
}

weak_ptr<Player> Planet::GetOwner() {
  if (m_OwnerId) {
    return m_Game.GetPlayer(m_OwnerId);
  }
  return weak_ptr<Player>();
}

void Planet::Colonize(int new_owner, uint32_t pop) {
  m_OwnerId = new_owner;
  m_Population.SetAmount(pop);
  SetProductProperties();
}

void Planet::SetProductProperties() {
  shared_ptr<Player> owner(GetOwner());
  // Existing factories (if any) remain
  m_Factories.SetMax(m_Population.GetMax() * owner->GetFactoriesPerPop());
}

/**
 * See http://realmsbeyond.net/forums/archive/index.php?thread-1905.html for lots of good info
 */
void Planet::Update() {
  weak_ptr<Player> weak_owner(GetOwner());
  if (weak_owner.expired()) {
    // Either not colonized, or bug.  Don't think it's possible to tell which.
    return;
  }
  // Grab a temporary pointer to the owner
  shared_ptr<Player> owner(weak_owner);
  uint32_t pop = m_Population.GetAmount();
  uint32_t activeFactories =
    std::min(m_Factories.GetAmount(), pop * owner->GetFactoriesPerPop());

  uint32_t capital = pop * owner->GetProductionPerPop() / 100;
  capital += activeFactories * owner->GetProductionPerFactory() / 100;

  m_Population.SetGrowthRate(NOMINAL_POP_GROWTH);
  m_Population.Grow(owner->GetPopCost(), 0);
  m_Factories.Grow(owner->GetFactoryCost(), uint32_t(floor(capital)));
}