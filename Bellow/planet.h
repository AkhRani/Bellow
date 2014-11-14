#include <stdint.h>
#include "product.h"

class Player;
extern "C" {
  struct lua_State;
}

/** Per-Planet info
* Collection of per-planet information
*/
class Planet {
  public:
    //! Basic constructor.  TODO: planetary class (minimal, hostile, etc)
    Planet(uint32_t maxPop);

    static Planet *Load(lua_State *L);

    //! Set or change the owner of a planet
    void Colonize(Player *owner, uint32_t pop);

    //! Current population.  Always <= GetMaxPopulation.
    uint32_t GetPopulation() { return m_population.GetAmount(); };
    uint32_t GetMaxPopulation() { return m_population.GetMax(); };

    uint32_t GetFactories() { return m_factories.GetAmount(); };

    //! Called once between turns
    void Update();

  protected:
    // double GetProduction();

  private:
    Player *m_pOwner;     //!< Current owner, may be null
    Product m_population; //!< Population
    Product m_factories;  //!< Factories
};
