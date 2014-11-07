#include <stdint.h>
#include "product.h"

class Player;

class Planet {
  public:
    Planet(uint32_t maxPop);

    void Colonize(Player *owner, uint32_t pop);
    uint32_t getPopulation() { return m_population.getAmount(); };
    uint32_t getMaxPopulation() { return m_population.getMax(); };

    uint32_t getFactories() { return m_factories.getAmount(); };

    void Update();

  protected:
    // double getProduction();

  private:
    Player *m_pOwner;
    Product m_population;
    Product m_factories;
};
