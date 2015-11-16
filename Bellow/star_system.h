#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

#include <string>

#include "planet.h"
#include "position.h"

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
  Position m_Position;
  int m_ID;

private:
  StarSystem();

  // All star systems have a planet.  "Empty" is an environment.
  Planet m_Planet;
};

#endif