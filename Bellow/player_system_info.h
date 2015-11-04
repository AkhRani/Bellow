#ifndef PLAYER_SYSTEM_INFO_H
#define PLAYER_SYSTEM_INFO_H

extern "C" {
  struct lua_State;
}

/**
* @brief Per-player star system information
* 
* This structure holds a player-specific view of the star system.
* Initially, only color and position are known.
*/
#include <string>
#include <stdint.h>

struct PlayerSystemInfo {
  //! System name.  Initially "?", set when explored.
  std::string name;

  // TODO:  Planet type
  //! Updated while owned or within fleet or scanner range.
  uint32_t factories, population;

  // TODO:  Missle Bases, shield, etc.
  void Save(std::string &rep);
  static PlayerSystemInfo Load(lua_State *L);
  PlayerSystemInfo(const std::string& name, uint32_t fact, uint32_t pop);
  PlayerSystemInfo();
};

#endif
