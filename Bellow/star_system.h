#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

/**
 * @brief Star system. 
 * aggregates position, star color, and planet (if any)
 */
#include <memory>
#include <string>

class Game;
class Planet;

extern "C" {
  struct lua_State;
}

class StarSystem {
public:
  StarSystem(const Game& game, lua_State *L);

  std::weak_ptr<Planet> GetPlanet();

  // TODO: Color
  std::string m_Name;
  double m_X, m_Y;

private:
  StarSystem();

  std::shared_ptr<Planet> m_Planet;
};

#endif