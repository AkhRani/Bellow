#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

extern "C" {
  struct lua_State;
}

/**
* @brief Star system information
* 
* This structure holds a player-specific view of the star system.
* Initially, only color and position are known.
*/
#include <string>

struct SystemInfo {
  // TODO:  Color
  //! System position.  Always correct.
  double x, y;

  //! System name.  Initially "?", set when explored.
  std::string name;

  // TODO:  Planet type
  //! Updated while owned or within fleet or scanner range.
  int factories, population;

  // TODO:  Missle Bases, shield, etc.
  void Save(lua_State *L, std::string &rep);
  static SystemInfo Load(lua_State *L);
};

#endif