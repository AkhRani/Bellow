#include "mock_game.h"

MockGame::MockGame() :
  m_Galaxy()
  , m_Player(new Player(m_Galaxy, "human")) {
}

Player* MockGame::GetPlayer(int playerId) const { return m_Player.get(); }