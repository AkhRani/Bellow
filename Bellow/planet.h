#ifndef PLANET_H
#define PLANET_H

#include <stdint.h>
#include <string>
#include "product.h"
#include "population.h"

class Player;
class IGame;

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
    Planet(IGame& game, uint32_t maxPop);

    //! Deserializing constructor
    Planet(IGame& game, lua_State *L, const char *fieldname = nullptr);

    //! Serializer
    void Save(std::string &rep);

    //! Resolve serialized reference to owner, etc.
    void FinishLoad() { SetProductProperties(); }

    //! Set or change the owner of a planet
    void Colonize(int playerId, uint32_t pop);

    //! Owner accessor, may return null
    Player* GetOwner();

    //! Current population.  Always <= GetMaxPopulation.
    uint32_t GetPopulation() { return m_Population.GetAmount(); };

    //! Current max population, modified by planetary improvements
    uint32_t GetMaxPopulation() { return m_Population.GetMax(); };

    uint32_t GetFactories() { return m_Factories.GetAmount(); };
    uint32_t GetMaxFactories() { return m_Factories.GetMax(); };

    //! Called once between turns
    void Update();

  protected:
    //! Update product cost, max.  Called for colonization, load, improvements, etc.
    void SetProductProperties();

  private:
    IGame& m_Game;
    uint32_t m_BasePop;   //!< Planet population cap without facilities
    int m_OwnerId;        //!< Current owner, -1 -> unowned
    Population m_Population; //!< Population
    Product m_Factories;  //!< Factories
};

#endif