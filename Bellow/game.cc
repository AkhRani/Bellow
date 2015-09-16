
#include "game.h"
// #include "star_system.h"
#include "system_info.h"
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

//! Boilerplate for lua functions
Game* Game::GetGalaxy(lua_State *L)
{
  Game* retval = nullptr;
  lua_getglobal(L, GAME_LUDNAME);
  if (lua_islightuserdata(L, -1)) {
    retval = (Game *)lua_touserdata(L, -1);
    lua_pop(L, 1);
  }
  else {
    lua_pop(L, 1);
    lua_pushnil(L);
  }
  return retval;
}

int Game::lua_GetGalaxySize(lua_State *L)
{
  Game *pGame = GetGalaxy(L);
  if (pGame) lua_pushnumber(L, pGame->GetGalaxySize());
  return 1;
}

int Game::lua_GetSystemCount(lua_State *L)
{
  Game *pGame = GetGalaxy(L);
  if (pGame) lua_pushnumber(L, pGame->GetSystemCount());
  return 1;
}

int Game::lua_GetSystemInfo(lua_State *L)
{
  Game *pGame = GetGalaxy(L);
  if (pGame && lua_isnumber(L, -1)) {
    int id = lua_tointeger(L, -1);
    SystemInfo info;
    // TODO:  Per-player info
    if (pGame->m_Galaxy.GetSystemInfo(id - 1, info)) {
      lua_createtable(L, 0, 3);

      lua_pushnumber(L, info.x);
      lua_setfield(L, -2, "x");

      lua_pushnumber(L, info.y);
      lua_setfield(L, -2, "y");

      lua_pushstring(L, info.name.c_str());
      lua_setfield(L, -2, "name");
    }
  }
  return 1;
}

bool Game::RegisterApi(lua_State *L)
{
  lua_register(L, "GetGalaxySize", lua_GetGalaxySize);
  lua_register(L, "GetSystemCount", lua_GetSystemCount);
  lua_register(L, "GetSystemInfo", lua_GetSystemInfo);

  // NOTE:  Could use pushcclosure instead of a global lightuserdata
  // Not sure which is better
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
  return m_Galaxy.SystemCount();
}

void Game::NextTurn()
{

}

void Game::UpdateSystemInfo()
{

}

//! Update player's view of the planet
void Game::Explore(Player& player, StarSystem& system)
{

}
