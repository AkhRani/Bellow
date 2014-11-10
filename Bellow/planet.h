#include <stdint.h>
#include "product.h"

class Player;

/** Per-Planet info
* Collection of per-planet information
*/
class Planet {
  public:
    //! Basic constructor.  TODO: planetary class (minimal, hostile, etc)
    Planet(uint32_t maxPop);

    //! Set or change the owner of a planet
    void Colonize(Player *owner, uint32_t pop);

    //! Current population.  Always <= getMaxPopulation.
    uint32_t getPopulation() { return m_population.getAmount(); };
    uint32_t getMaxPopulation() { return m_population.getMax(); };

    uint32_t getFactories() { return m_factories.getAmount(); };

    //! Called once between turns
    void Update();

  protected:
    // double getProduction();

  private:
    Player *m_pOwner;     //!< Current owner, may be null
    Product m_population; //!< Population
    Product m_factories;  //!< Factories
};
