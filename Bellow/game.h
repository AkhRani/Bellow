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

  virtual std::weak_ptr<Player> GetPlayer(const std::string &playerName) const override;

  virtual double GetGalaxySize() const;
  virtual int GetSystemCount() const;
  virtual void NextTurn();

protected:
  virtual void UpdateSystemInfo();

  //! Update player's view of the planet
  virtual void Explore(Player& player, System& system);

private:
  Game();

  std::vector<Player> m_Players;
  Galaxy m_Galaxy;

  // TODO:  Fleets, tech tree, etc.

};
#endif