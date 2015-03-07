#include <memory>
#include <string>

#include "game.h"

extern "C" {
  struct lua_State;
}

void RunLua(lua_State *L, const char *script);

class MockGame : public Game {
public:
  MockGame();
  virtual std::weak_ptr<Player> GetPlayer(const std::string &name) const;

private:
  std::shared_ptr<Player> m_Player;
};

