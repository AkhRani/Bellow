#include <math.h>
#include "product.h"

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
