#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

#include <memory>
#include <string>

class IGame;
class Planet;

extern "C" {
  struct lua_State;
}

/**
* @brief Star system.
* aggregates position, star color, and planet (including "Empty")
*/
class StarSystem {
public:
  StarSystem(IGame& game, lua_State *L, int id);
  void Save(std::string& rep);
  void FinishLoad();

  void NextTurn();
  std::weak_ptr<Planet> GetPlanet();

  // TODO: Color
  std::string m_Name;
  double m_X, m_Y;
  int m_ID;

private:
  StarSystem();

  // TODO:  All star systems have a planet.  "Empty" is an environment.
  std::shared_ptr<Planet> m_Planet;
};

/**
* @brief Mapping from system ID to system
* This interface is used to deserialize references to star systems,
* for fleet destinations and (possibly) player colony lists.
* It is implemented by Galaxy in the actual game,
* and by mock objects in the unit tests.
*/
class IStarSystemOwner {
public:
  virtual ~IStarSystemOwner() {};

  //! Returns pointer to system.
  // Destruction order must guarantee systems outlive clients,
  // Or client destructors must not use the pointers.
  virtual StarSystem* GetStarSystem(int systemId) = 0;
};

#endif