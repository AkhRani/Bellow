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
Planet::Planet(IGame &game, lua_State *L, const char *fieldname) :
  m_Game(game)
  , m_BasePop(0)
  , m_OwnerId(0)
  , m_Population(0, 0)
  , m_Factories(0, 0)
{
  if (fieldname) {
    lua_getfield(L, -1, fieldname);
  }

  if (lua_istable(L, -1)) {
    m_BasePop = LoadCheckInteger(L, "base_population");

    lua_getfield(L, -1, "population");
    m_Population.Load(L);

    lua_getfield(L, -1, "factories");
    m_Factories.Load(L);

    m_Population.SetMax(m_BasePop);

    m_OwnerId = LoadOptInteger(L, "owner", 0);
  }
  lua_pop(L, 1);
}

void Planet::Save(string &rep) {
  rep.append("\n{ name = \"");
  rep.append("dummy\"");
  rep.append(", base_population = " + to_string(m_BasePop));
  rep.append(", population =");
  m_Population.Save(rep);
  rep.append(", factories =");
  m_Factories.Save(rep);
  rep.append(" }");
}

Player* Planet::GetOwner() {
  if (m_OwnerId) {
    return m_Game.GetPlayer(m_OwnerId);
  }
  return nullptr;
}

void Planet::Colonize(int new_owner, uint32_t pop) {
  m_OwnerId = new_owner;
  m_Population.SetAmount(pop);
  SetProductProperties();
}

void Planet::SetProductProperties() {
  Player* owner(GetOwner());
  if (owner) {
    // Existing factories (if any) remain
    m_Factories.SetMax(m_Population.GetMax() * owner->GetFactoriesPerPop());
  }
}

/**
 * See http://realmsbeyond.net/forums/archive/index.php?thread-1905.html for lots of good info
 */
void Planet::Update() {
  Player* owner(GetOwner());
  if (!owner) {
    // Either not colonized, or bug.  Don't think it's possible to tell which.
    return;
  }
  uint32_t pop = m_Population.GetAmount();
  uint32_t activeFactories =
    std::min(m_Factories.GetAmount(), pop * owner->GetFactoriesPerPop());

  uint32_t capital = pop * owner->GetProductionPerPop() / 100;
  capital += activeFactories * owner->GetProductionPerFactory() / 100;

  m_Population.SetGrowthRate(NOMINAL_POP_GROWTH);
  m_Population.Grow(owner->GetPopCost(), 0);
  m_Factories.Grow(owner->GetFactoryCost(), uint32_t(floor(capital)));
}