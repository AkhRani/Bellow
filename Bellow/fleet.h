#ifndef FLEET_H
#define FLEET_H

#include <string>
#include "position.h"

extern "C" {
  struct lua_State;
}

class Player;
class IGalaxy;

//! Collection of ships with a common destination
//
// For now, each "fleet" is identical, conceptually equivalent to a scout ship
// Eventually, will consist of shipCount[SHIP_DESIGNS].
class Fleet {
public:
  Fleet(Player& owner, IGalaxy& systemOwner, lua_State *L);
  Fleet(Player& owner, IGalaxy& systemOwner, int system);
  void Save(std::string &rep);

  // TODO:  "Position" class?
  void GetPosition(double &x, double &y);
  void SetDestination(int system);
  void Move();
  void Approach();
  void Arrive();

  enum FleetState {
    ST_ORBITING,      //!< In orbit, no destination
    ST_LAUNCHING,     //!< Destination given, has not moved yet
    ST_TRAVELING,     //!< Moving towards destination
    ST_APPROACHING,   //!< Transient state, may trigger combat  (Not sure if we really need this state.)
    ST_ARRIVING       //!< Transient state, may trigger exploration
  };

  // State accessors
  bool IsInOrbit() { return ST_ORBITING == m_State; }
  int GetOrbitSystem() { return ST_ORBITING == m_State ? m_Orbit : -1; }
  bool IsLaunching() { return ST_LAUNCHING == m_State; }
  bool IsApproaching() { return ST_APPROACHING == m_State; }
  bool IsArriving() { return ST_ARRIVING == m_State; }

private:
  Player& m_Owner;
  IGalaxy& m_Galaxy;
  // TODO:  double m_Speed;
  FleetState m_State;
  Position m_Position;
  int m_Orbit;    //!< Star system we are orbiting (if InOrbit or Launching)
  int m_Target;   //!< Star system we are going to (if not InOrbit)
};

#endif