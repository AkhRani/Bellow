#include <math.h>
#include <string>

#include "product.h"
#include "util.h"

extern "C" {
#include "lua.h"
}

using std::string;

/** Load state from Lua
 * Structure: { amount = 10, fractional = 0. }
 */
void Product::Load(lua_State *L) {
  LoadCheck(lua_istable(L, -1));
  m_amount = LoadCheckInteger(L, "amount");
  // Max should be set by client code, but do this to maintain invariant
  // in the mean time.  TODO:  Maybe pass in max?
  m_max = m_amount;
  m_fractional = LoadCheckDouble(L, "fractional");
  LoadCheck(m_fractional <= 1.0);

  lua_pop(L, 1);
}

void Product::Save(string &serialized) {
  // growth rate, cost, and max will be recalculated on load
  serialized.append(" { ");
  serialized.append(" amount=" + std::to_string(m_amount));
  serialized.append(", fractional=" + std::to_string(m_fractional));
  serialized.append(" }");
}

void Product::Grow(double capital) {
  if (m_amount < m_max) {
    double Growth = m_growthRate * (m_amount + m_fractional) + m_fractional;
    if (GetCost() > 0.) {
      Growth += capital / GetCost();
    }

    double intGrowth = floor(Growth);
    m_fractional = Growth - intGrowth;
    // TODO:  Prevent overflow
    m_amount += intGrowth;
    LimitAmount();
  }
}

void Product::LimitAmount() {
  if (m_amount > m_max) {
    m_amount = m_max;
    m_fractional = 0.;
  }
}

void Product::SetAmount(uint32_t amount)
{
  m_amount = amount;
  m_fractional = 0.;
  LimitAmount();
}

void Product::SetMax(uint32_t max)
{
  m_max = max;
  LimitAmount();
}
