#ifndef GALAXY_H
#define GALAXY_H

#include <string>
#include <vector>

#include "planet.h"
#include "position.h"

extern "C" {
  struct lua_State;
}

/** Collection of star systems
*
* The galaxy object owns the star systems.
*/
class Galaxy {
public:
  //! Simple aggregation of star system info
  class StarSystem {
  public:
    explicit StarSystem(lua_State *L);
    StarSystem(const std::string &name, const Position &pos, Planet *pPlanet) :
      m_Name(name), m_Position(pos), m_pPlanet(pPlanet)
    {}
    
    const std::string& GetName() const { return m_Name; }
    const Position& GetPosition() const { return m_Position;}
    Planet* GetPlanet() { return m_pPlanet; }

  private:
    StarSystem();

    // TODO:  Add star color
    std::string m_Name;
    Position m_Position;
    Planet* m_pPlanet;
  };

  typedef std::vector<StarSystem> StarSystemColl;
  typedef StarSystemColl::iterator StarSystemIter;

  Galaxy(); //!< Default constructor
  void AddStarSystem(const std::string &name, const Position &pos, Planet *pPlanet);
  void AddStarSystem(const std::string &name, Planet *pPlanet);

  StarSystemIter BeginSystems() { return m_Systems.begin(); }
  StarSystemIter EndSystems() { return m_Systems.end(); }
  int SystemCount() { return m_Systems.size(); }               //!< Number of systems in the galaxy

private:
  StarSystemColl m_Systems;
};

#endif