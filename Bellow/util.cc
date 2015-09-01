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

double LoadCheckDouble(lua_State *L, const char *name) {
  int success;
  lua_getfield(L, -1, name);
  double retval = lua_tonumberx(L, -1, &success);
  lua_pop(L, 1);
  LoadCheck(success != 0);
  return retval;
}
