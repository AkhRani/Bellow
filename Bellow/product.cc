#include <math.h>
#include <string>

using std::string;

#include "product.h"

extern "C" {
#include "lua.h"
}

void Product::Load(lua_State *L) {
  int success;
  lua_getfield(L, -1, "max");
  m_max = lua_tointegerx(L, -1, &success);
  lua_pop(L, 1);
}

void Product::Save(string &serialized) {
  // growth rate, cost, and (eventually) max will be recalculated on load
  serialized.append(" { ");
  serialized.append(" amount=" + std::to_string(m_amount));
  serialized.append(", fractional=" + std::to_string(m_fractional));
  serialized.append(", max=" + std::to_string(m_max));
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
