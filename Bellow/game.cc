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
using std::unique_ptr;
using namespace std::placeholders;

//! Lua user-data name for game object
const char* Game::GAME_LUDNAME = "Bellow_Game";


void Game::LoadPlayer(lua_State *L, int idx) {
  m_Players.push_back(unique_ptr<Player>(new Player(m_Galaxy, L)));
}


// Top of Lua stack should be a game table
Game::Game(lua_State *L) :
  m_Turn(1)
  , m_CurrentPlayer(0)
  , m_Players()
  , m_Galaxy(*this, L, "galaxy")
{
  LoadTableOfTables(L, "players", bind(&Game::LoadPlayer, this, _1, _2));
  FinishLoad();
  UpdateSystemInfo();
  // TODO:  UpdateVisibleFleets for each player
}

void Game::FinishLoad() {
  m_Galaxy.FinishLoad();
}

class UpdateSystemInfoVisitor : public Galaxy::SystemVisitor {
public:
  UpdateSystemInfoVisitor(Game* pGame, Player* pPlayer) : m_pGame(pGame), m_pPlayer(pPlayer) {}
  ~UpdateSystemInfoVisitor() {}

  virtual void operator ()(StarSystem &system) override {
    bool owned(false);
    if (auto planet = system.GetPlanet().lock()) {
      if (auto owner = planet->GetOwner()) {
        if (owner == m_pPlayer) {
          // Owner gets full information.  TODO:  Correct pop / fact
          SystemInfo info{ system.m_X, system.m_Y, system.m_Name, 10, 10 };
          owner->SetSystemInfo(system.m_ID, info);
          owned = true;
        }
      }
    }
    if (!owned) {
      SystemInfo info{ system.m_X, system.m_Y, "?", 0, 0 };
      m_pPlayer->SetSystemInfo(system.m_ID, info);
    }
  }

private:
  Game* m_pGame;
  Player* m_pPlayer;
};


//! Make sure player system info is up-to-date.
//
// For now, this is only called after the game is loaded,
// to make things easier for the game creation code.
//
void Game::UpdateSystemInfo() {
  // Owned planets
  for (auto &player : m_Players) {
    UpdateSystemInfoVisitor v(this, player.get());
    m_Galaxy.VisitSystems(v);
  }
  // TODO: Long-range / planetary sensors
  // TODO: Short-range / ship sensors
}


Game::~Game() {
}


int Game::GetFleetCount() {
  return m_Players.at(m_CurrentPlayer)->GetFleetCount();
}

Fleet& Game::GetFleet(int fleet) {
  return m_Players.at(m_CurrentPlayer)->GetFleet(fleet);
}

//! Launch the given fleet to the given destination (zero-based)
bool Game::SetFleetDestination(unsigned int fleet, unsigned int system) {
  return m_Players.at(m_CurrentPlayer)->SetFleetDestination(fleet, system);
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


int Game::lua_GetSystemInfo(lua_State *L) {
  Game *pGame = GetGame(L);
  if (pGame && lua_isnumber(L, -1)) {
    int id = lua_tointeger(L, -1);
    SystemInfo info;
    // TODO:  try/catch for invalid ID
    pGame->CurrentPlayer().GetSystemInfo(id, info);
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


int Game::lua_GetFleetCount(lua_State *L) {
  Game *pGame = GetGame(L);
  if (pGame) lua_pushnumber(L, pGame->GetFleetCount());
  return 1;
}


int Game::lua_GetFleetInfo(lua_State *L) {
  Game *pGame = GetGame(L);
  if (pGame && lua_isnumber(L, -1)) {
    int fleetId = lua_tointeger(L, -1);
    if (fleetId > 0 && fleetId <= pGame->GetFleetCount()) {
      double x, y;
      Fleet& fleet = pGame->CurrentPlayer().GetFleet(fleetId - 1);
      fleet.GetPosition(x, y);
      lua_createtable(L, 0, 2);

      lua_pushnumber(L, x);
      lua_setfield(L, -2, "x");

      lua_pushnumber(L, y);
      lua_setfield(L, -2, "y");
    }
  }
  return 1;
}

int Game::lua_SetFleetDestination(lua_State *L) {
  Game *pGame = GetGame(L);
  int success = 0;
  if (pGame && lua_isnumber(L, -1) && lua_isnumber(L, -2)) {
    int system = lua_tointeger(L, -1);
    int fleet = lua_tointeger(L, -2);
    if (fleet > 0 && fleet <= pGame->GetFleetCount() &&
        system > 0 && system <= pGame->GetSystemCount()) {
      if (pGame->SetFleetDestination(fleet - 1, system - 1)) {
        success = 1;
      }
    }
  }
  lua_pushnumber(L, success);
  return 1;
}

bool Game::RegisterApi(lua_State *L) {
  lua_register(L, "GetPlayerCount", lua_GetPlayerCount);
  lua_register(L, "GetGalaxySize", lua_GetGalaxySize);
  lua_register(L, "GetSystemCount", lua_GetSystemCount);
  lua_register(L, "GetSystemInfo", lua_GetSystemInfo);
  lua_register(L, "GetFleetCount", lua_GetFleetCount);
  lua_register(L, "GetFleetInfo", lua_GetFleetInfo);
  lua_register(L, "SetFleetDestination", lua_SetFleetDestination);
  lua_register(L, "EndTurn", lua_EndTurn);

  // NOTE:  Could use pushcclosure instead of a global lightuserdata
  // Not sure which is better
  lua_pushlightuserdata(L, this);
  lua_setglobal(L, GAME_LUDNAME);
  return true;
}

Player* Game::GetPlayer(int playerId) const {
  if (playerId > 0 && size_t(playerId) <= m_Players.size()) {
    return m_Players[playerId-1].get();
  }
  assert(false);
  // TODO:  generate warning / error for scripts
  return nullptr;
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
  virtual void operator ()(StarSystem &system) override {
    system.NextTurn();
  }
};


// All players have completed the current turn.  Update.
void Game::NextTurn() {
  // Launch transports

  // Construction
  NextTurnSystemVisitor visitor;
  m_Galaxy.VisitSystems(visitor);

  // Move Fleets / transports
  for (auto& player : m_Players) {
    player->MoveFleets();
  }

  // TODO: Resolve Battles
}


//! Update player's view of the planet
void Game::Explore(Player& player, StarSystem& system) {

}
