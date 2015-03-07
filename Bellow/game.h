#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>

class Player;
class Galaxy;

/**
* @brief Game interface
* This is an interface so the unit tests can easily provide a mock implementation.
*/
class Game {
public:
  virtual ~Game() {};

  virtual std::weak_ptr<Player> GetPlayer(const std::string &playerName) const = 0;
};

#endif