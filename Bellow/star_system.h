#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

#include <string>
#include "Planet.h"

class IGame;

extern "C" {
  struct lua_State;
}

/**
* @brief Star system.
* aggregates position, star color, and planet (including "Empty")
*/
class StarSystem {
public:
  //! Standard deserializer constructor
  StarSystem(IGame& game, lua_State *L, int id);

  //! Create empty star system for testing purposes
  StarSystem(IGame& game, int id, double x, double y);

  void Save(std::string& rep);
  void FinishLoad();

  void NextTurn();
  Planet& GetPlanet();

  // TODO: Color
  std::string m_Name;
  // TODO:  Const?
  double m_X, m_Y;
  int m_ID;

private:
  StarSystem();

  // All star systems have a planet.  "Empty" is an environment.
  Planet m_Planet;
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