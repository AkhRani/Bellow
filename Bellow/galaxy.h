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

/**! Mapping from system ID to system.
* It is implemented by Galaxy in the actual game,
* and by mock objects in the unit tests. */
class IGalaxy {
public:
  virtual ~IGalaxy() {};

  /**! Returns (non-owning) pointer to system.
  * This function is used to deserialize references to star systems,
  * for fleet destinations and (possibly) player colony lists.
  * Destruction order must guarantee systems outlive clients,
  * Or client destructors must not use the pointers. */
  virtual StarSystem* GetStarSystem(int systemId) = 0;
};


/** Collection of star systems
*
* The galaxy object owns the star systems.
*/
class Galaxy : public IGalaxy {
public:

  Galaxy(IGame& game, lua_State *L, const char *field = nullptr); //!< deserializer constructor
  void Save(std::string& rep);  //!< Serializer
  void FinishLoad();

  typedef std::function<void(StarSystem&)> SystemVisitor;
  void VisitSystems(SystemVisitor &visitor);
  bool Galaxy::GetSystemInfo(unsigned int id, SystemInfo& info);

  //! Size of the (square) galaxy, in parsecs
  double Size() const { return m_Size; }

  //! Number of systems in the galaxy
  unsigned int GetSystemCount() const { return m_Systems.size(); }
  StarSystem* GetStarSystem(int id) override;

protected:
  typedef std::vector<StarSystem> StarSystemColl;
  typedef StarSystemColl::iterator StarSystemIter;

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