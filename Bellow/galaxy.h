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

  Galaxy(IGame& game, lua_State *L, const char *field = nullptr); //!< deserializer constructor

  void AddStarSystem(const std::string &name, const Position &pos, Planet *pPlanet);
  void AddStarSystem(const std::string &name, Planet *pPlanet);
  int VisitPlanets(SystemVisitor &visitor);

  StarSystemIter BeginSystems() { return m_Systems.begin(); }
  StarSystemIter EndSystems() { return m_Systems.end(); }
  double Size() const { return m_Size; }
  int SystemCount() const { return m_Systems.size(); }               //!< Number of systems in the galaxy

private:
  //! Default constructor
  Galaxy();

  //! Dimension of (square) galaxy, in parsecs
  double m_Size;
  StarSystemColl m_Systems;
};

#endif