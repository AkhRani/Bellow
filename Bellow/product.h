#include <stdint.h>
#include <string>

extern "C" {
  struct lua_State;
}

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

    // TODO:  Maybe get rid of this
    explicit Product(uint32_t max)
      : m_growthRate(0.),
        m_cost(0.),
        m_fractional(0.),
        m_amount(0),
        m_max(max) {}

    explicit Product(const Product &other)
      : m_growthRate(other.m_growthRate),
      m_cost(other.m_cost),
      m_fractional(other.m_fractional),
      m_amount(other.m_amount),
      m_max(other.m_max) {}

    Product(double growthRate, double cost, uint32_t amount, uint32_t max)
      : m_growthRate(growthRate),
        m_cost(cost),
        m_fractional(0.),
        m_amount(amount),
        m_max(max) {}

    //! Deserializer
    void Load(lua_State *L);

    //! Serializer
    void Save(std::string &serialized);

    void SetAmount(uint32_t amount);    // Sets fractional amount to zero.
    void SetMax(uint32_t max);          // POST(GetAmount() <= GetMax())
    void SetCost(double cost) { m_cost = cost; }
    void SetGrowthRate(double rate) { m_growthRate = rate; }

    uint32_t GetAmount() { return m_amount; };
    uint32_t GetMax() { return m_max; };
    double GetCost() { return m_cost; };
    double GetGrowthRate() { return m_growthRate; }
    uint32_t ProjectGrowth(double capital);
    void Grow(double capital);

  protected:
    void LimitAmount();

  private:
    double m_growthRate;
    double m_cost;
    double m_fractional;
    uint32_t m_amount;
    uint32_t m_max;
};