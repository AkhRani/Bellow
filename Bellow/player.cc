#include <assert.h>

#include "player.h"
#include "star_system.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


using std::string;
using std::bind;
using namespace std::placeholders;

#define POP_GROWTH_RATE 100
#define POP_COST 20
#define PRODUCTION_PER_POP 50

#define FACTORIES_PER_POP 2
#define PRODUCTION_PER_FACTORY 100
#define FACTORY_COST 10

Player::Player(IStarSystemOwner& galaxy, const string &name) :
  m_SystemOwner(galaxy)
  , m_Name(name) {}

Player::Player(IStarSystemOwner& galaxy, lua_State *L) :
  m_SystemOwner(galaxy)
  , m_Name(LoadString(L, "name"))
{
  // TODO:  Pass Galaxy to fleets to resolve destinations
  LoadTableOfTables(L, "fleets", bind(&Player::LoadFleet, this, _1, _2));
  LoadTableOfTables(L, "systems", bind(&Player::LoadSystemInfo, this, _1, _2));
  lua_pop(L, 1);
}

void Player::LoadFleet(lua_State *L, int idx) {
  m_Fleets.push_back(Fleet(*this, m_SystemOwner, L));
}

void Player::LoadSystemInfo(lua_State *L, int idx) {
  m_SystemInfo.push_back(SystemInfo::Load(L));
}

void Player::Save(string &rep) {
  rep.append("\n  { name=\"");
  rep.append(m_Name);
  rep.append("\",\n   fleets={");
  for (auto& v : m_Fleets) {
    v.Save(rep);
  }
  rep.append("},\n   systems={");
  for (auto& v : m_SystemInfo) {
    v.Save(rep);
  }
  rep.append("}\n  }");
}


//! Direct the given fleet to the given system
// @param fleet Fleet ID, 1-based (TODO)
// @param system System ID, 1-based
bool Player::SetFleetDestination(unsigned int fleet, unsigned int system) {
  bool retval(false);
  // Precondition, failure indicates bug.
  assert(system < m_SystemInfo.size() && fleet < GetFleetCount());
  if (system < m_SystemInfo.size() && fleet < GetFleetCount()) {
    // TODO:  1-based fleet ID, Check Range
    auto& f = m_Fleets[fleet];
    f.SetDestination(system);
    retval = true;
  }
  else {
    // TODO: Possible AI script error, log?
  }
  return retval;
}


void Player::MoveFleets() {
  for (auto& fleet : m_Fleets) {
    fleet.Move();
  }
}

uint32_t Player::GetPopCost() {
  return POP_COST;
}

uint32_t Player::GetPopGrowthRate() {
  return POP_GROWTH_RATE;
}

uint32_t Player::GetProductionPerPop() {
  return PRODUCTION_PER_POP;
}

uint32_t Player::GetFactoriesPerPop() {
  return FACTORIES_PER_POP;
}

uint32_t Player::GetFactoryCost() {
  return FACTORY_COST;
}

uint32_t Player::GetProductionPerFactory() {
  return PRODUCTION_PER_FACTORY;
}


//! Get the player's view of the given system
//
// Note that system IDs are one-based
void Player::GetSystemInfo(unsigned int id, SystemInfo& info) const {
  if (0 < id && id <= m_SystemInfo.size()) {
    info = m_SystemInfo[id-1];
  }
}

//! Explore the given system
//
// This is called when a fleet arrives at a star system.
// If the player has not previously explored this system, this should
// queue a notification for the player.
void Player::Explore(unsigned int systemId) {
  assert(systemId >= 1);
  StarSystem *pSystem = m_SystemOwner.GetStarSystem(systemId);
  assert(pSystem);
  SystemInfo info{ pSystem->m_X, pSystem->m_Y, pSystem->m_Name, 0, 0 };
  SetSystemInfo(systemId, info);
}

//! Update the player's view of the given system
//
// Note that system IDs are one-based
void Player::SetSystemInfo(unsigned int id, const SystemInfo& info) {
  // TODO:  If new info has same or more fields, set up new fields and "staleness"
  if (id >= 1) {
    if (id > m_SystemInfo.size()) {
      m_SystemInfo.resize(id);
      m_SystemInfo[id - 1] = info;
    }
    else {
      // TODO:  Selectively update info?
      m_SystemInfo[id - 1] = info;
    }
  }
}