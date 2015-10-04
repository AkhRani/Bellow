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
  bool InOrbit() { return m_Orbiting; }

private:
  const Player& m_Owner;
  double m_X, m_Y;
  double m_DestX, m_DestY;
  double m_Speed;
  bool m_Orbiting;
};

#endif