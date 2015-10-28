#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <string>

#include "galaxy.h"
#include "player.h"

extern "C" {
  struct lua_State;
}

class StarSystem;
class Galaxy;

/**
* @brief Game interface
* This is an interface so the unit tests can easily provide a mock implementation.
*/
class IGame {
public:
  virtual ~IGame() {};

  // TODO:  Make sure destruction order can guarantee players outlive clients.
  virtual Player* GetPlayer(int playerId) const = 0;
};

/**
 * @brief Standard game implementation
 *
 * Game construction order:
 * 1. Galaxy, with star systems and planets, owner IDs
 * 2. Players, with fleets, star system IDs for destinations
 *
 * Game destruction order:
 * 1. Player fleets (removes pointers to star systems)
 * 2. Galaxy (removes pointers to Players)
 * 3. Players
 */
class Game : public IGame {
public:
  Game(lua_State *L);
  virtual ~Game();
  //! Resolve serialized references
  void FinishLoad();

  virtual Player* GetPlayer(int playerId) const override;

  int GetPlayerCount() const { return m_Players.size(); }
  double GetGalaxySize() const;
  int GetSystemCount() const;

  // Current Player Functions
  void GetSystemInfo(int id, SystemInfo& info);
  int GetFleetCount();
  Fleet& GetFleet(int fleet);
  bool SetFleetDestination(unsigned int fleet, unsigned int system);
  int GetExplorationEventCount();
  int GetExplorationEvent(int id);
  void EndPlayerTurn();

protected:
  void LoadPlayer(lua_State *L, int idx);
  Player& CurrentPlayer() { return *m_Players[m_CurrentPlayer]; }
  void NextTurn();
  void UpdateSystemInfo();

private:
  Game();

  unsigned int m_Turn;
  unsigned int m_CurrentPlayer;
  std::vector<std::unique_ptr<Player>> m_Players;
  Galaxy m_Galaxy;

  // TODO:  Fleets, tech tree, etc.

};
#endif