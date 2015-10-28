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
using namespace std::placeholders;


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

// TODO:  return info or pass reference.  Pick one.

void Game::GetSystemInfo(int id, SystemInfo& info) {
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
  m_Players.at(m_CurrentPlayer)->EndTurn();
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

  // TODO: Resolve Battles and "Arrive" fleets.

  // Explore systems
  for (auto& player : m_Players) {
    // Placeholder
    player->HandleFleetArrival();
  }
}