#include "util.h"

extern "C" {
#include "lua.h"
}

const char* LoadString(lua_State *L, const char *name) {
  lua_getfield(L, -1, name);
  const char *retval = lua_tostring(L, -1);
  lua_pop(L, 1);
  return retval;
}

int LoadCheckInteger(lua_State *L, const char *name) {
  int success;
  lua_getfield(L, -1, name);
  int retval = lua_tointegerx(L, -1, &success);
  lua_pop(L, 1);
  LoadCheck(success != 0);
  return retval;
}

int LoadOptInteger(lua_State *L, const char *name, int default) {
  int success(1), retval;
  lua_getfield(L, -1, name);
  if (lua_isnil(L, -1)) {
    retval = default;
  }
  else {
    retval = lua_tointegerx(L, -1, &success);
  }
  lua_pop(L, 1);
  LoadCheck(success != 0);
  return retval;
}

double LoadCheckDouble(lua_State *L, const char *name) {
  int success;
  lua_getfield(L, -1, name);
  double retval = lua_tonumberx(L, -1, &success);
  lua_pop(L, 1);
  LoadCheck(success != 0);
  return retval;
}

/*
double LoadOptDouble(lua_State *L, const char *name, double default) {
  int success(1);
  double retval;
  lua_getfield(L, -1, name);
  if (lua_isnil(L, -1)) {
    retval = default;
  }
  else {
    retval = lua_tonumberx(L, -1, &success);
  }
  lua_pop(L, 1);
  LoadCheck(success != 0);
  return retval;
}
*/

void LoadTableOfTables(lua_State *L, const char* pField, std::function<void(lua_State*, int)> callback) {
  lua_getfield(L, -1, pField);
  if (lua_istable(L, -1)) {
    int idx = 1;
    while (1) {
      int top = lua_gettop(L);
      lua_rawgeti(L, -1, idx);
      if (lua_istable(L, -1)) {
        callback(L, idx - 1);   // zero-based ID
      }
      else {
        lua_pop(L, 1);
        break;
      }
      idx++;
    }
  }
  lua_pop(L, 1);
}