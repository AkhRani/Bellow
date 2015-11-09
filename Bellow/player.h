#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include <string>
#include <vector>

#include "system_info.h"
#include "player_system_info.h"
#include "fleet.h"

class IStarSystemOwner;

extern "C" {
  struct lua_State;
}

/** Per-player information.
* This class will encapsulate or aggregate racials, tech advances,
* which systems have been explored, etc.
*/
class Player {
public:
  // Construction and serialization
  Player(IStarSystemOwner& galaxy, const std::string &name, unsigned int id = 0);
  Player(IStarSystemOwner& galaxy, lua_State *L, unsigned int id = 0);
  void Save(std::string &rep);

  // Fleet management
  // TODO:  Fleet type and count
  unsigned int CreateFleet(unsigned int systemId);
  unsigned int GetFleetCount() const { return m_Fleets.size(); };
  Fleet& GetFleet(int fleet) { return m_Fleets.at(fleet); }
  bool SetFleetDestination(unsigned int fleet, unsigned int system);
  bool Colonize(unsigned int systemId);
  void MoveFleets();
  // (deprecated)
  void HandleFleetArrival();

  // Game-generated events

  // TODO:  We have multiple types of notifications.  What's the best way
  // to deal with that?
  int GetExplorationEventCount();
  int GetExplorationEvent(int id);

  // Player Attributes
  const std::string &GetName() const { return m_Name; }
  unsigned int GetId() const { return m_Id; }
  uint32_t GetPopCost();
  uint32_t GetPopGrowthRate();
  uint32_t GetProductionPerPop();
  uint32_t GetFactoriesPerPop();
  uint32_t GetFactoryCost();
  uint32_t GetProductionPerFactory();

  // Functions to manage player's view of the galaxy
  void SetSystemCount(unsigned int count);
  void Explore(unsigned int systemId);
  void GetSystemInfo(unsigned int systemId, SystemInfo& info) const;
  void SetSystemInfo(unsigned int systemId, const PlayerSystemInfo& info);

  // Misc
  void EndTurn();

protected:
  void LoadFleet(lua_State *L, int idx);
  void LoadSystemInfo(lua_State *L, int idx);

private:
  Player();
  IStarSystemOwner& m_SystemOwner;
  std::string m_Name;
  const unsigned int m_Id;

  //! Fleets owned by the player
  std::vector<Fleet> m_Fleets;

  //! Most recent surveys of all star systems
  std::vector<PlayerSystemInfo> m_SystemInfo;

  //! Exploration Notifications
  std::vector<int> m_ExploredSystems;
};

#endif