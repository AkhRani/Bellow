#include "game.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

Game::Game(lua_State *L) {
  // Top of Lua stack should be a gane table
  if (lua_istable(L, -1)) {
    lua_getfield(L, -1, "players");
    lua_pop(L, 1);
#if 0
    m_size = LoadCheckDouble(L, "size");
    int idx = 1;
    while (1) {
      int top = lua_gettop(L);
      lua_rawgeti(L, -1, idx);
      if (lua_istable(L, -1)) {
        StarSystem sys(game, L);
        if (sys.m_X >= 0. && sys.m_X <= m_size &&
          sys.m_Y >= 0. && sys.m_Y <= m_size) {
          m_Systems.push_back(sys);
        }
        else {
          // TODO:  Load warning / error
        }
        idx++;
      }
      else {
        lua_pop(L, 1);
        break;
      }
    }
#endif
  }
}

Game::~Game() {

}

std::weak_ptr<Player> Game::GetPlayer(const std::string &playerName) const
{
  static std::shared_ptr<Player> dummy(new Player("human"));
  return dummy;
}

double Game::GetGalaxySize() const {
  return 123.;
}

int Game::GetSystemCount() const
{
  return 48;
}

void Game::NextTurn()
{

}

void Game::UpdateSystemInfo()
{

}

//! Update player's view of the planet
void Game::Explore(Player& player, System& system)
{

}
