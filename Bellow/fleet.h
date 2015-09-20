#ifndef FLEET_H
#define FLEET_H

extern "C" {
  struct lua_State;
}

class Player;
class StarSystem;

//! Collection of ships with a common destination
//
// For now, each "fleet" is identical, conceptually equivelent to a scout ship
// Eventually, will consist of shipCount[SHIP_DESIGNS].
class Fleet {
public:
  Fleet(const Player& owner, lua_State *L);
  Fleet(const Player& owner, double x, double y);
  void SetDestination(StarSystem& system);
  bool InOrbit() { return m_X == m_DestX && m_Y == m_DestY;  }

private:
  double m_X, m_Y;
  double m_DestX, m_DestY;
  double m_Speed;
  const Player& m_Owner;
};

#endif