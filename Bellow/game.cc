#include "game.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

const char* Game::GAME_LUDNAME = "Bellow_Game";

Game::PlayerColl::PlayerColl(lua_State *L) {
  lua_getfield(L, -1, "players");
  // TODO:  Maybe refactor lua table iteration into util
  if (lua_istable(L, -1)) {
    int idx = 1;
    while (1) {
      int top = lua_gettop(L);
      lua_rawgeti(L, -1, idx);
      if (lua_istable(L, -1)) {
        push_back(Player(L));
      }
      else {
        lua_pop(L, 1);
        break;
      }
      idx++;
    }
  }
  // TODO:  Throw if no players found
  lua_pop(L, 1);
}

// Top of Lua stack should be a gane table
Game::Game(lua_State *L) :
  m_Players(L),
  m_Galaxy(*this, L, "galaxy")
{
}

Game::~Game() {
}

int Game::lua_GetGalaxySize(lua_State *L)
{
  lua_getglobal(L, GAME_LUDNAME);
  if (lua_islightuserdata(L, -1)) {
    Game *pGame = (Game *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    lua_pushnumber(L, pGame->GetGalaxySize());
  }
  else {
    lua_pop(L, 1);
    lua_pushnil(L);
  }
  return 1;
}

bool Game::RegisterApi(lua_State *L)
{
  lua_register(L, "GetGalaxySize", lua_GetGalaxySize);

  lua_pushlightuserdata(L, this);
  lua_setglobal(L, GAME_LUDNAME);
  return true;
}

std::weak_ptr<Player> Game::GetPlayer(const std::string &playerName) const
{
  static std::shared_ptr<Player> dummy(new Player("human"));
  return dummy;
}

double Game::GetGalaxySize() const {
  return m_Galaxy.Size();
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
