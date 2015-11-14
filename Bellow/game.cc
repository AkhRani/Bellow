#include "assert.h"
#include "game.h"
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
typedef Galaxy::SystemVisitor SystemVisitor;
using namespace std::placeholders;


void Game::LoadPlayer(lua_State *L, int idx) {
  m_Players.push_back(unique_ptr<Player>(new Player(m_Galaxy, L, idx)));
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
  for (auto& player : m_Players) {
    player->SetSystemCount(m_Galaxy.GetSystemCount());
  }
}

//! Make sure player system info is up-to-date.
//
// For now, this is only called after the game is loaded,
// to make things easier for the game creation code.
//
void Game::UpdateSystemInfo() {
  // Owned planets
  SystemVisitor SetSystemInfo([this] (StarSystem& system) {
    auto& planet(system.GetPlanet());
    if (auto owner = planet.GetOwner()) {
      PlayerSystemInfo info{ system.m_Name, planet.GetFactories(), planet.GetPopulation() };
      owner->SetSystemInfo(system.m_ID, info);
    }
  });
  m_Galaxy.VisitSystems(SetSystemInfo);

  // TODO: Long-range / planetary sensors
  // TODO: Short-range / ship sensors
}


Game::~Game() {
}


void Game::GetSystemInfo(int id, SystemInfo& info) {
  m_Galaxy.GetSystemInfo(id, info);
  CurrentPlayer().GetSystemInfo(id, info);
}


int Game::GetFleetCount() {
  return m_Players.at(m_CurrentPlayer)->GetFleetCount();
}


Fleet& Game::GetFleet(int fleet) {
  return m_Players.at(m_CurrentPlayer)->GetFleet(fleet);
}


//! Launch the given fleet to the given destination (one-based)
bool Game::SetFleetDestination(unsigned int fleet, unsigned int system) {
  return m_Players.at(m_CurrentPlayer)->SetFleetDestination(fleet, system);
}


int Game::GetExplorationEventCount() {
  return m_Players.at(m_CurrentPlayer)->GetExplorationEventCount();
}


int Game::GetExplorationEvent(int id) {
  return m_Players.at(m_CurrentPlayer)->GetExplorationEvent(id);
}


Player* Game::GetPlayer(int id) const {
  if (CheckId(id, m_Players)) {
    return m_Players[id].get();
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
  m_Players.at(m_CurrentPlayer)->EndTurn();
  m_CurrentPlayer++;
  if (m_CurrentPlayer >= m_Players.size()) {
    NextTurn();
    m_CurrentPlayer = 0;
  }
}


// All players have completed the current turn.  Update.
void Game::NextTurn() {
  // Construction
  m_Galaxy.VisitSystems(SystemVisitor([] (StarSystem& system) { system.NextTurn(); }));

  // Move Fleets
  for (auto& player : m_Players) {
    player->MoveFleets();
  }

  // TODO: Resolve Battles and "Arrive" fleets.

  // Move transports, ground combat

  // Explore systems
  for (auto& player : m_Players) {
    // Placeholder
    player->HandleFleetArrival();
  }

  // Scan systems
  UpdateSystemInfo();
}