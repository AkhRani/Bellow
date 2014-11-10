#include <math.h>
#include "product.h"

void Product::grow(double capital) {
  if (m_amount < m_max) {
    double growth = m_growthRate * (m_amount + m_fractional) + m_fractional;
    if (getCost() > 0.) {
      growth += capital / getCost();
    }

    double intGrowth = floor(growth);
    m_fractional = growth - intGrowth;
	// TODO:  Prevent overflow
    m_amount += intGrowth;
    limitAmount();
  }
}

void Product::limitAmount() {
  if (m_amount > m_max) {
    m_amount = m_max;
    m_fractional = 0.;
  }
}

void Product::setAmount(uint32_t amount)
{
  m_amount = amount;
  m_fractional = 0.;
  limitAmount();
}

void Product::setMax(uint32_t max)
{
  m_max = max;
  limitAmount();
}
