#include "player.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define POP_GROWTH_RATE 100
#define POP_COST 20
#define PRODUCTION_PER_POP 50

#define FACTORIES_PER_POP 2
#define PRODUCTION_PER_FACTORY 100
#define FACTORY_COST 10

Player::Player(const std::string &name) : m_Name(name) {}

Player::Player(lua_State *L) :
  m_Name(LoadString(L, "name"))
{
  lua_pop(L, 1);
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

//! Update the player's view of the given system
//
// Note that system IDs are one-based
void Player::SetSystemInfo(unsigned int id, const SystemInfo& info) {
  if (id >= 1) {
    if (id > m_SystemInfo.size()) {
      m_SystemInfo.resize(id);
    }
    m_SystemInfo[id - 1] = info;
  }
}