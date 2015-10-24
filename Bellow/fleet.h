#ifndef FLEET_H
#define FLEET_H

#include <string>

extern "C" {
  struct lua_State;
}

class Player;
class IStarSystemOwner;

//! Collection of ships with a common destination
//
// For now, each "fleet" is identical, conceptually equivelent to a scout ship
// Eventually, will consist of shipCount[SHIP_DESIGNS].
class Fleet {
public:
  Fleet(Player& owner, IStarSystemOwner& systemOwner, lua_State *L);
  Fleet(Player& owner, IStarSystemOwner& systemOwner, int system);
  void Save(std::string &rep);

  // TODO:  "Position" class?
  void GetPosition(double &x, double &y);
  void SetDestination(int system);
  //! First stage of fleet movement
  void Move();
  //! Second stage of fleet movement
  void Approach();
  //! Third stage of fleet movement
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
  bool IsLaunching() { return ST_LAUNCHING == m_State; }
  bool IsApproaching() { return ST_APPROACHING == m_State; }
  bool IsArriving() { return ST_ARRIVING == m_State; }

private:
  Player& m_Owner;
  IStarSystemOwner& m_SystemOwner;
  // TODO:  double m_Speed;
  FleetState m_State;
  double m_X, m_Y;
  int m_Orbit;    //!< Star system we are orbiting (if InOrbit or Launching)
  int m_Target;   //!< Star system we are going to (if not InOrbit)
};

#endif