#include <assert.h>

#include "galaxy.h"
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

//! Constructor for testing purposes.  Might get rid of this
Player::Player(IGalaxy& galaxy, const string &name, unsigned int id) :
  m_SystemOwner(galaxy)
  , m_Name(name)
  , m_Id(id) 
{}

//! Deserializer constructor
Player::Player(IGalaxy& galaxy, lua_State *L, unsigned int id) :
  m_SystemOwner(galaxy)
  , m_Name(LoadString(L, "name"))
  , m_Id(id)
{
  // TODO:  Pass Galaxy to fleets to resolve destinations
  LoadTableOfTables(L, "fleets", bind(&Player::LoadFleet, this, _1, _2));
  LoadTableOfTables(L, "systems", bind(&Player::LoadSystemInfo, this, _1, _2));
  lua_pop(L, 1);
}

//! Deserializer helper
void Player::LoadFleet(lua_State *L, int idx) {
  m_Fleets.push_back(Fleet(*this, m_SystemOwner, L));
}

//! Deserializer helper
void Player::LoadSystemInfo(lua_State *L, int idx) {
  m_SystemInfo.push_back(PlayerSystemInfo::Load(L));
}

//! Serializer
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


//! End-of-turn cleanup
void Player::EndTurn() {
  m_ExploredSystems.clear();
}


//! Create new fleet at the given system
unsigned int Player::CreateFleet(unsigned int systemId) {
  size_t nextFleet = m_Fleets.size();
  m_Fleets.push_back(Fleet(*this, m_SystemOwner, systemId));
  return nextFleet;
}


//! Direct the given fleet to the given system
// @param fleet Fleet ID, 1-based
// @param system System ID, 1-based
bool Player::SetFleetDestination(unsigned int fleet, unsigned int system) {
  assert (CheckId(system, m_SystemInfo) && CheckId(fleet, m_Fleets));
  bool retval(false);
  if (CheckId(system, m_SystemInfo) && CheckId(fleet, m_Fleets)) {
    auto& f = m_Fleets[fleet];
    f.SetDestination(system);
    retval = true;
  }
  else {
    // TODO: Possible AI script error, log?
  }
  return retval;
}


//! Create a colony, if possible
bool Player::Colonize(unsigned int systemId) {
  // Sanity check
  StarSystem *pSystem = m_SystemOwner.GetStarSystem(systemId);
  if (pSystem != nullptr && pSystem->GetPlanet().GetOwner() == nullptr) {
    // TODO:  Check system's list of orbiting fleets.
    for (auto& fleet : m_Fleets) {
      if (fleet.IsInOrbit() && fleet.GetOrbitSystem() == systemId) {
        pSystem->GetPlanet().Colonize(m_Id, 1);
        return true;
      }
    }
  }
  return false;
}

//! Update all fleet positions
void Player::MoveFleets() {
  for (auto& fleet : m_Fleets) {
    fleet.Move();
  }
}

//! Deprecated
void Player::HandleFleetArrival() {
  for (auto& fleet : m_Fleets) {
    fleet.Arrive();
  }
}

//! Ecological investment to "build" one population
uint32_t Player::GetPopCost() {
  return POP_COST;
}

//! New population per unit of population, in percent
uint32_t Player::GetPopGrowthRate() {
  return POP_GROWTH_RATE;
}

//! Capital generated by each unit of population, in percent (100 = 1 BC per pop)
uint32_t Player::GetProductionPerPop() {
  return PRODUCTION_PER_POP;
}

//! Number of factories operated by one unit of population
uint32_t Player::GetFactoriesPerPop() {
  return FACTORIES_PER_POP;
}

//! Cost of a factory, in BC
uint32_t Player::GetFactoryCost() {
  return FACTORY_COST;
}

//! Capital generated by one factory (100 = 1 BC per fact)
uint32_t Player::GetProductionPerFactory() {
  return PRODUCTION_PER_FACTORY;
}


/**! Get the player's view of the given system.
 *
 * @param id    System ID, one-based
 * @param info  Set to player's system info if ID is valid
 */
void Player::GetSystemInfo(unsigned int id, SystemInfo& info) const {
  if (CheckId(id, m_SystemInfo)) {
    const PlayerSystemInfo& myInfo = m_SystemInfo[id];
    info.name = myInfo.name;
    info.factories = myInfo.factories;
    info.population = myInfo.population;
    // TODO:  last updated turn
  }
}

/**! Explore the given system.
 * This is called when a fleet arrives at a star system.
 */
void Player::Explore(unsigned int id) {
  assert(id >= 0);
  StarSystem *pSystem = m_SystemOwner.GetStarSystem(id);
  assert(pSystem);
  // If the player has not previously explored this system,
  if (CheckId(id, m_SystemInfo)) {
    PlayerSystemInfo& myInfo = m_SystemInfo[id];
    if (myInfo.name != pSystem->m_Name) {
      // queue a notification for the player.
      m_ExploredSystems.push_back(id);

      // And record exploration
      myInfo.name = pSystem->m_Name;

      // TODO:  Game will set factories and population based on presence of fleet
      auto& planet = pSystem->GetPlanet();
      myInfo.factories = planet.GetFactories();
      myInfo.population = planet.GetPopulation();
    }
  }
}

//! Get number of systems explored this turn
int Player::GetExplorationEventCount() {
  return m_ExploredSystems.size();
}

//! Get (zero-based) ID of system explored, -1 on bad parameter
int Player::GetExplorationEvent(int id) {
  if (CheckId(id, m_ExploredSystems)) {
    return m_ExploredSystems.at(id);
  }
  return -1;
}

/**! Allocate space for star system info, if needed.
 * This should only need to be called after initial game creation.
 * Normally, player-specific system information is set by the deserializer,
 * which should have a full list of data.
 */
void Player::SetSystemCount(unsigned int count) {
  // Normally only be called once, number of systems must not change
  assert(count == m_SystemInfo.size() || 0 == m_SystemInfo.size());
  m_SystemInfo.resize(count);
}


//! Update the player's view of the given system
//
// TODO:  Store the game turn as well, to track how old the info is.
void Player::SetSystemInfo(unsigned int id, const PlayerSystemInfo& info) {
  assert(CheckId(id, m_SystemInfo));
  if (CheckId(id, m_SystemInfo)) {
      m_SystemInfo[id] = info;
  }
}