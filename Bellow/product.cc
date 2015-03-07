#include <math.h>
#include <string>

#include "product.h"
#include "util.h"

extern "C" {
#include "lua.h"
}

using std::string;

/** Load state from Lua
 * Structure: { amount = 10, invested = 0 }
 */
void Product::Load(lua_State *L) {
  LoadCheck(lua_istable(L, -1));
  m_amount = LoadCheckInteger(L, "amount");
  // Max should be set by client code, but do this to maintain invariant
  // in the mean time.  TODO:  Maybe pass in max?
  m_max = m_amount;
  // Invested amount should be < cost, but this will be handled when cost is set.
  m_invested = LoadCheckInteger(L, "invested");

  lua_pop(L, 1);
}

void Product::Save(string &serialized) {
  // growth rate, cost, and max will be recalculated on load
  serialized.append(" { ");
  serialized.append(" amount=" + std::to_string(m_amount));
  serialized.append(", invested=" + std::to_string(m_invested));
  serialized.append(" }");
}

void Product::Grow(uint32_t cost, uint32_t capital) {
  if (m_amount < m_max) {
    capital += m_invested;
    uint32_t growth = capital / cost;
    // Remainder
    m_invested = capital - growth * cost;
    m_amount += growth;
    LimitAmount();
  }
}

void Product::LimitAmount() {
  if (m_amount > m_max) {
    // TODO:  Send excess capital to owner
    m_amount = m_max;
    m_invested = 0;
  }
}

void Product::SetAmount(uint32_t amount)
{
  m_amount = amount;
  m_invested = 0;
  LimitAmount();
}

void Product::SetMax(uint32_t max)
{
  m_max = max;
  LimitAmount();
}
