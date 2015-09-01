#ifndef GALAXY_H
#define GALAXY_H

#include <string>
#include <vector>

#include "planet.h"
#include "position.h"
#include "star_system.h"

extern "C" {
  struct lua_State;
}

/** Collection of star systems
*
* The galaxy object owns the star systems.
*/
class Galaxy {
public:
  typedef std::vector<StarSystem> StarSystemColl;
  typedef StarSystemColl::iterator StarSystemIter;

  class SystemVisitor {
  public:
    SystemVisitor();
    ~SystemVisitor();
    virtual int operator ()(StarSystem &system) { return 1; }
  };

  Galaxy(); //!< Default constructor
  Galaxy(Game& game, lua_State *L); //!< deserializer constructor

  void AddStarSystem(const std::string &name, const Position &pos, Planet *pPlanet);
  void AddStarSystem(const std::string &name, Planet *pPlanet);
  int VisitPlanets(SystemVisitor &visitor);

  StarSystemIter BeginSystems() { return m_Systems.begin(); }
  StarSystemIter EndSystems() { return m_Systems.end(); }
  int SystemCount() { return m_Systems.size(); }               //!< Number of systems in the galaxy

private:
  StarSystemColl m_Systems;
};

#endif