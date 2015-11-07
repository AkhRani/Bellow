#include "game.h"
#include "lua_api.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace {

  //! Lua user-data name for game object
  const char* GAME_LUDNAME = "Bellow_Game";

  //! Boilerplate for lua functions
  Game* GetGame(lua_State *L)
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


  int lua_GetPlayerCount(lua_State *L)
  {
    Game *pGame = GetGame(L);
    if (pGame) lua_pushnumber(L, pGame->GetPlayerCount());
    return 1;
  }


  int lua_GetGalaxySize(lua_State *L)
  {
    Game *pGame = GetGame(L);
    if (pGame) lua_pushnumber(L, pGame->GetGalaxySize());
    return 1;
  }


  int lua_GetSystemCount(lua_State *L)
  {
    Game *pGame = GetGame(L);
    if (pGame) lua_pushnumber(L, pGame->GetSystemCount());
    return 1;
  }


  int lua_GetSystemInfo(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame && lua_isnumber(L, -1)) {
      int id = lua_tointeger(L, -1);
      SystemInfo info;
      // TODO:  try/catch for invalid ID
      pGame->GetSystemInfo(id, info);
      lua_createtable(L, 0, 5);

      lua_pushnumber(L, info.x);
      lua_setfield(L, -2, "x");

      lua_pushnumber(L, info.y);
      lua_setfield(L, -2, "y");

      lua_pushstring(L, info.name.c_str());
      lua_setfield(L, -2, "name");

      lua_pushnumber(L, info.factories);
      lua_setfield(L, -2, "fact");

      lua_pushnumber(L, info.population);
      lua_setfield(L, -2, "pop");
    }
    return 1;
  }


  int lua_EndTurn(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame) {
      pGame->EndPlayerTurn();
    }
    return 0;
  }


  int lua_GetFleetCount(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame) lua_pushnumber(L, pGame->GetFleetCount());
    return 1;
  }


  int lua_GetFleetInfo(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame && lua_isnumber(L, -1)) {
      int fleetId = lua_tointeger(L, -1);
      if (fleetId > 0 && fleetId <= pGame->GetFleetCount()) {
        double x, y;
        Fleet& fleet = pGame->GetFleet(fleetId - 1);
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

  int lua_SetFleetDestination(lua_State *L) {
    Game *pGame = GetGame(L);
    int success = 0;
    if (pGame && lua_isnumber(L, -1) && lua_isnumber(L, -2)) {
      int system = lua_tointeger(L, -1);
      int fleet = lua_tointeger(L, -2);
      if (pGame->SetFleetDestination(fleet, system)) {
        success = 1;
      }
    }
    lua_pushnumber(L, success);
    return 1;
  }

  int lua_GetExplorationEventCount(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame) lua_pushnumber(L, pGame->GetExplorationEventCount());
    return 1;
  }

  int lua_GetExplorationEvent(lua_State *L) {
    Game *pGame = GetGame(L);
    if (pGame && lua_isnumber(L, -1)) {
      int id = lua_tointeger(L, -1);
      lua_pushnumber(L, pGame->GetExplorationEvent(id));
    }
    return 1;
  }

} // namespace

bool RegisterApi(lua_State *L, Game *pGame) {
  lua_register(L, "GetPlayerCount", lua_GetPlayerCount);
  lua_register(L, "GetGalaxySize", lua_GetGalaxySize);
  lua_register(L, "GetSystemCount", lua_GetSystemCount);
  lua_register(L, "GetSystemInfo", lua_GetSystemInfo);
  lua_register(L, "GetFleetCount", lua_GetFleetCount);
  lua_register(L, "GetFleetInfo", lua_GetFleetInfo);
  lua_register(L, "SetFleetDestination", lua_SetFleetDestination);
  lua_register(L, "GetExplorationEventCount", lua_GetExplorationEventCount);
  lua_register(L, "GetExplorationEvent", lua_GetExplorationEvent);
  lua_register(L, "EndTurn", lua_EndTurn);

  // NOTE:  Could use pushcclosure instead of a global lightuserdata
  // Not sure which is better
  lua_pushlightuserdata(L, pGame);
  lua_setglobal(L, GAME_LUDNAME);
  return true;
}
