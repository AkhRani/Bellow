#ifndef GALAXY_H
#define GALAXY_H

#include <string>
#include <vector>
#include <functional>

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
  
  typedef std::function<void(StarSystem&)> SysVisitor;

  Galaxy(IGame& game, lua_State *L, const char *field = nullptr); //!< deserializer constructor
  void Save(std::string& rep);  //!< Serializer
  void FinishLoad();

  void VisitSystems(SystemVisitor &visitor);
  void VisitSystems(SysVisitor &visitor);
  bool Galaxy::GetSystemInfo(unsigned int id, SystemInfo& info);

  //! Size of the (square) galaxy, in parsecs
  double Size() const { return m_Size; }

  //! Number of systems in the galaxy
  unsigned int GetSystemCount() const { return m_Systems.size(); }
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