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

struct SystemInfo;

/** Collection of star systems
*
* The galaxy object owns the star systems.
*/
class Galaxy : public IStarSystemOwner {
public:
  typedef std::vector<StarSystem> StarSystemColl;
  typedef StarSystemColl::iterator StarSystemIter;

  class SystemVisitor {
  public:
    SystemVisitor() {};
    virtual ~SystemVisitor() {};
    virtual void operator ()(StarSystem &system) = 0;
  };

  Galaxy(IGame& game, lua_State *L, const char *field = nullptr); //!< deserializer constructor
  void Save(std::string& rep);  //!< Serializer
  void FinishLoad();

  void AddStarSystem(const std::string &name, const Position &pos, Planet *pPlanet);
  void AddStarSystem(const std::string &name, Planet *pPlanet);
  void VisitSystems(SystemVisitor &visitor);

  //! Size of the (square) galaxy, in parsecs
  double Size() const { return m_Size; }

  //! Number of systems in the galaxy
  unsigned int GetSystemCount() const { return m_Systems.size(); }
  // bool GetSystemInfo(unsigned int id, SystemInfo& info);
  //! Deserialize reference
  StarSystem* GetStarSystem(int id) override;

protected:
  void LoadSystem(lua_State *L, int idx);

private:
  //! Default constructor
  Galaxy();

  //! Currently only used during loading
  IGame& m_Game;

  //! Size of the (square) galaxy, in parsecs
  double m_Size;
  StarSystemColl m_Systems;
};

#endif