#include <stdint.h>

/** Base class for things that are built / grow on planets.
 *
 */
class Product {
  public:
    Product()
      : m_growthRate(0.),
        m_cost(0.),
        m_fractional(0.),
        m_amount(0),
        m_max(0) {}

    explicit Product(uint32_t max)
      : m_growthRate(0.),
        m_cost(0.),
        m_fractional(0.),
        m_amount(0),
        m_max(max) {}

    Product(double growthRate, double cost, uint32_t amount, uint32_t max)
      : m_growthRate(growthRate),
        m_cost(cost),
        m_fractional(0.),
        m_amount(amount),
        m_max(max) {}

    void setAmount(uint32_t amount);    // Sets fractional amount to zero.
    void setMax(uint32_t max);          // POST(getAmount() <= getMax())
    void setCost(double cost) { m_cost = cost; }
    void setGrowthRate(double rate) { m_growthRate = rate; }

    uint32_t getAmount() { return m_amount; };
    uint32_t getMax() { return m_max; };
    double getCost() { return m_cost; };
    double getGrowthRate() { return m_growthRate; }
    uint32_t projectGrowth(double capital);
    void grow(double capital);

  protected:
    void limitAmount();

  private:
    double m_growthRate;
    double m_cost;
    double m_fractional;
    uint32_t m_amount;
    uint32_t m_max;
};
