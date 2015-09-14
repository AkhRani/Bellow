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

class System;
class Galaxy;

/**
* @brief Game interface
* This is an interface so the unit tests can easily provide a mock implementation.
*/
class IGame {
public:
  virtual ~IGame() {};

  virtual std::weak_ptr<Player> GetPlayer(const std::string &playerName) const = 0;
};

/**
 * @brief Standard game implementation
 */
class Game : public IGame {
public:
  Game(lua_State *L);
  virtual ~Game();

  bool RegisterApi(lua_State *L);
  virtual std::weak_ptr<Player> GetPlayer(const std::string &playerName) const override;

  double GetGalaxySize() const;
  int GetSystemCount() const;
  void NextTurn();

protected:
  class PlayerColl : public std::vector < Player > {
  public:
    PlayerColl(lua_State *L);
  };

  void UpdateSystemInfo();

  //! Update player's view of the planet
  void Explore(Player& player, System& system);

  // Lua API
  static const char* GAME_LUDNAME;

  static int lua_GetGalaxySize(lua_State *L);
  static int lua_GetSystemCount(lua_State *L);
  static int lua_GetSystemInfo(lua_State *L);

private:
  Game();

  PlayerColl m_Players;
  Galaxy m_Galaxy;

  // TODO:  Fleets, tech tree, etc.

};
#endif