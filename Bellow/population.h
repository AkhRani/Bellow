#ifndef POPULATION_H
#define POPULATION_H

#include "product.h"
class Population :
  public Product
{
public:
  Population();
  explicit Population(uint32_t max) : Product(max) {}
  Population(uint32_t amount, uint32_t max) : Product(amount, max) {}
  ~Population();

  void SetGrowthRate(uint32_t rate) { m_growthRate = rate; }    // Percentage?
  virtual void Grow(uint32_t cost, uint32_t capital);

private:
  uint32_t m_growthRate;
};

#endif