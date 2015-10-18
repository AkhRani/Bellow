#ifndef FLEET_H
#define FLEET_H

#include <string>

extern "C" {
  struct lua_State;
}

class Player;

//! Collection of ships with a common destination
//
// For now, each "fleet" is identical, conceptually equivelent to a scout ship
// Eventually, will consist of shipCount[SHIP_DESIGNS].
class Fleet {
public:
  Fleet(const Player& owner, lua_State *L);
  Fleet(const Player& owner, double x, double y);
  void Save(std::string &rep);

  // TODO:  "Position" class?
  void GetPosition(double &x, double &y);
  void SetDestination(double x, double y);
  void Move();

  enum FleetState {
    ST_ORBITING,      //!< In orbit, no destination
    ST_LAUNCHING,     //!< Destination given, has not moved yet
    ST_TRAVELING,     //!< Moving towards destination
    ST_APPROACHING,   //!< Transient state, may trigger combat
    ST_ARRIVING       //!< Transient state, may trigger exploration
  };

  // State accessors
  bool InOrbit() { return ST_ORBITING == m_State; }
  bool Launching() { return ST_LAUNCHING == m_State; }

private:
  const Player& m_Owner;
  double m_X, m_Y;
  double m_DestX, m_DestY;
  double m_Speed;
  FleetState m_State;
};

#endif