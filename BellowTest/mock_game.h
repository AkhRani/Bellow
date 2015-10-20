#ifndef MOCK_GAME_H
#define MOCK_GAME_H

#include <memory>
#include "game.h"
#include "mock_galaxy.h"

class MockGame : public IGame {
public:
  MockGame();
  virtual Player* GetPlayer(int playerId) const override;
  MockGalaxy m_Galaxy;

private:
  std::unique_ptr<Player> m_Player;
};

#endif