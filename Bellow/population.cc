#include "population.h"


Population::Population()
{
}


Population::~Population()
{
}

void Population::Grow(uint32_t cost, uint32_t invested)
{
  // Nearly m_growthRate with pop 1, 0 with max pop.
  // Source:  http://forums.civfanatics.com/showthread.php?t=84644
  // Note:  adding max / 2 for rounding.
  uint32_t max = GetMax();
  uint32_t scaledGrowthRate = (max / 2 + m_growthRate * (max - GetAmount())) / max;
  uint32_t growth = cost * scaledGrowthRate * GetAmount() / 100;
  Product::Grow(cost, invested + growth);
}
