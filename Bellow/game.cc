#include "assert.h"
#include "game.h"
// #include "star_system.h"
#include "system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;
using namespace std::placeholders;

const char* Game::GAME_LUDNAME = "Bellow_Game";

void Game::PlayerColl::LoadPlayer(lua_State *L, int idx) {
  push_back(shared_ptr<Player>(new Player(L)));
}


Game::PlayerColl::PlayerColl(lua_State *L) {
  LoadTableOfTables(L, "players", bind(&Game::PlayerColl::LoadPlayer, this, _1, _2));
  // TODO:  Throw if no players found
}


// Top of Lua stack should be a game table
Game::Game(lua_State *L) :
  m_Turn(1)
  , m_Players(L)
  , m_Galaxy(*this, L, "galaxy")
{
  UpdateSystemInfo();
  // TODO:  UpdateVisibleFleets for each player
}


class UpdateSystemInfoVisitor : public Galaxy::SystemVisitor {
public:
  UpdateSystemInfoVisitor(Game *pGame, shared_ptr<Player> player) : m_pGame(pGame), m_Player(player) {}
  ~UpdateSystemInfoVisitor() {}

  virtual int operator ()(StarSystem &system) {
    bool owned(false);
    if (auto planet = system.GetPlanet().lock()) {
      if (auto owner = planet->GetOwner().lock()) {
        if (owner == m_Player) {
          // Owner gets full information
          SystemInfo info{ system.m_X, system.m_Y, system.m_Name, 10, 10 };
          owner->SetSystemInfo(system.m_ID, info);
          owned = true;
        }
      }
    }
    if (!owned) {
      SystemInfo info{ system.m_X, system.m_Y, "?", 0, 0 };
      m_Player->SetSystemInfo(system.m_ID, info);
    }
    return 1;
  }

private:
  Game *m_pGame;
  shared_ptr<Player> m_Player;
};


//! Make sure player system info is up-to-date.
//
// For now, this is only called after the game is loaded,
// to make things easier for the game creation code.
//
void Game::UpdateSystemInfo() {
  // Owned planets
  for (auto player : m_Players) {
    UpdateSystemInfoVisitor v(this, player);
    m_Galaxy.VisitSystems(v);
  }
  // TODO: Long-range / planetary sensors
  // TODO: Short-range / ship sensors
}


Game::~Game() {
}


//! Launch the given fleet to the given destination (zero-based)
bool Game::SetFleetDestination(unsigned int fleet, unsigned int system) {
  auto player = m_Players[m_CurrentPlayer];
  return player->SetFleetDestination(fleet, system);
}


//! Boilerplate for lua functions
Game* Game::GetGame(lua_State *L)
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


int Game::lua_GetPlayerCount(lua_State *L)
{
  Game *pGame = GetGame(L);
  if (pGame) lua_pushnumber(L, pGame->GetPlayerCount());
  return 1;
}


int Game::lua_GetGalaxySize(lua_State *L)
{
  Game *pGame = GetGame(L);
  if (pGame) lua_pushnumber(L, pGame->GetGalaxySize());
  return 1;
}


int Game::lua_GetSystemCount(lua_State *L)
{
  Game *pGame = GetGame(L);
  if (pGame) lua_pushnumber(L, pGame->GetSystemCount());
  return 1;
}


int Game::lua_GetSystemInfo(lua_State *L)
{
  Game *pGame = GetGame(L);
  if (pGame && lua_isnumber(L, -1)) {
    int id = lua_tointeger(L, -1);
    // TODO:  Active player
    SystemInfo info;
    // TODO:  try/catch for invalid ID
    pGame->m_Players[0]->GetSystemInfo(id, info);
    lua_createtable(L, 0, 3);

    lua_pushnumber(L, info.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, info.y);
    lua_setfield(L, -2, "y");

    lua_pushstring(L, info.name.c_str());
    lua_setfield(L, -2, "name");
  }
  return 1;
}


int Game::lua_EndTurn(lua_State *L) {
  Game *pGame = GetGame(L);
  if (pGame) {
    pGame->EndPlayerTurn();
  }
  return 0;
}


bool Game::RegisterApi(lua_State *L) {
  lua_register(L, "GetPlayerCount", lua_GetPlayerCount);
  lua_register(L, "GetGalaxySize", lua_GetGalaxySize);
  lua_register(L, "GetSystemCount", lua_GetSystemCount);
  lua_register(L, "GetSystemInfo", lua_GetSystemInfo);
  lua_register(L, "EndTurn", lua_EndTurn);

  // NOTE:  Could use pushcclosure instead of a global lightuserdata
  // Not sure which is better
  lua_pushlightuserdata(L, this);
  lua_setglobal(L, GAME_LUDNAME);
  return true;
}

weak_ptr<Player> Game::GetPlayer(const string &playerName) const {
  for (auto player : m_Players) {
    if (player->GetName() == playerName) {
      return weak_ptr<Player>(player);
    }
  }
  assert(false);
  return weak_ptr<Player>();
}


double Game::GetGalaxySize() const {
  return m_Galaxy.Size();
}


int Game::GetSystemCount() const {
  return m_Galaxy.GetSystemCount();
}

// End the turn for the current player
void Game::EndPlayerTurn() {
  m_CurrentPlayer++;
  if (m_CurrentPlayer >= m_Players.size()) {
    NextTurn();
    m_CurrentPlayer = 0;
  }
}


class NextTurnSystemVisitor : public Galaxy::SystemVisitor {
public:
  virtual int operator ()(StarSystem &system) override {
    system.NextTurn();
    return 1;
  }
};


// All players have completed the current turn.  Update.
void Game::NextTurn() {
  // Launch transports

  // Construction
  NextTurnSystemVisitor visitor;
  m_Galaxy.VisitSystems(visitor);

  // Move Fleets / transports
  for (auto player : m_Players) {
    player->MoveFleets();
  }

  // TODO: Resolve Battles
}


//! Update player's view of the planet
void Game::Explore(Player& player, StarSystem& system) {

}
