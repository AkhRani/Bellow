#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdint.h>
#include <string>

extern "C" {
  struct lua_State;
}

/** Base class for things that are built / grow on planets.
 */
class Product {
  public:
    Product()
      : m_invested(0),
        m_amount(0),
        m_max(0) {}

    // TODO:  Maybe get rid of this
    explicit Product(uint32_t max)
      : m_invested(0),
        m_amount(0),
        m_max(max) {}

    Product(uint32_t amount, uint32_t max)
      : m_invested(0),
        m_amount(amount),
        m_max(max) {}

    //! Deserializer
    void Load(lua_State *L);

    //! Serializer
    void Save(std::string &rep);

    void SetAmount(uint32_t amount);    //!< Note: Sets invested amount to zero.
    void SetMax(uint32_t max);

    uint32_t GetAmount() { return m_amount; };
    uint32_t GetInvested() { return m_invested; }  //!< For unit testing
    uint32_t GetMax() { return m_max; };
    uint32_t ProjectGrowth(uint32_t cost, uint32_t capital);
    virtual void Grow(uint32_t cost, uint32_t capital);

  protected:
    void LimitAmount();

  private:
    uint32_t m_invested;    //!< Amount of capital invested toward the next item
    uint32_t m_amount;    //!< Number of items
    uint32_t m_max;       //!< Cached maximum number of items
};

#endif