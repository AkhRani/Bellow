#ifndef PLANET_H
#define PLANET_H

#include <stdint.h>
#include <string>
#include <memory>
#include "product.h"
#include "population.h"

class Player;
class Game;

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
    Planet(const Game &game, lua_State *L);

    //! Serializer
    void Save(std::string &rep);

    //! Deserializer, Factory style.  Returns planet or throws.
    static Planet *Load(const Game &game, lua_State *L);

    //! Set or change the owner of a planet
    void Colonize(std::weak_ptr<Player> owner, uint32_t pop);

    //! Owner accessor
    std::weak_ptr<Player> GetOwner() { return m_owner; };

    //! Current population.  Always <= GetMaxPopulation.
    uint32_t GetPopulation() { return m_population.GetAmount(); };

    //! Current max population, modified by planetary improvements
    uint32_t GetMaxPopulation() { return m_population.GetMax(); };

    uint32_t GetFactories() { return m_factories.GetAmount(); };
    uint32_t GetMaxFactories() { return m_factories.GetMax(); };

    //! Called once between turns
    void Update();

  protected:
    //! Update product cost, max.  Called for colonization, load, improvements, etc.
    void SetProductProperties();

  private:
    uint32_t m_basePop;   //!< Planet population cap without facilities
    std::weak_ptr<Player> m_owner;     //!< Current owner, may be null
    Population m_population; //!< Population
    Product m_factories;  //!< Factories
};

#endif