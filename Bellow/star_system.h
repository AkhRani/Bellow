#ifndef STAR_SYSTEM_H
#define STAR_SYSTEM_H

/**
 * @brief Star system. 
 * aggregates position, star color, and planet (including "Empty")
 */
#include <memory>
#include <string>

class IGame;
class Planet;

extern "C" {
  struct lua_State;
}

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

  std::shared_ptr<Planet> m_Planet;
};

#endif