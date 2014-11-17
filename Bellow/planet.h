#include <stdint.h>
#include <string>
#include "product.h"

class Player;
extern "C" {
  struct lua_State;
}

/** Per-Planet info
 * Stuff on planets is divided into two categories, products and improvements.
 * Products are multiple copies of the same thing, while improvements are
 * singular.  Population, factories, and missile bases are products.  Soil
 * Enrichment, Terraforming, and planetary shields are improvements.
 */
class Planet {
  public:
    //! Basic constructor.  TODO: planetary class (minimal, hostile, etc)
    Planet(uint32_t maxPop);

    //! Deserializing constructor
    Planet(lua_State *L);

    //! Serializer
    void Save(std::string &rep);

    //! Deserializer, Factory style.  Returns planet or throws.
    static Planet *Load(lua_State *L);

    //! Set or change the owner of a planet
    void Colonize(Player *owner, uint32_t pop);

    //! Current population.  Always <= GetMaxPopulation.
    uint32_t GetPopulation() { return m_population.GetAmount(); };
    uint32_t GetMaxPopulation() { return m_population.GetMax(); };

    uint32_t GetFactories() { return m_factories.GetAmount(); };
    uint32_t GetMaxFactories() { return m_factories.GetMax(); };

    //! Called once between turns
    void Update();

  protected:
    // double GetProduction();

  private:
    uint32_t m_basePop;   //!< Planet population cap without facilities
    Player *m_pOwner;     //!< Current owner, may be null
    Product m_population; //!< Population
    Product m_factories;  //!< Factories
};
