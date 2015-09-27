#include "system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;

SystemInfo SystemInfo::Load(lua_State *L) {
  SystemInfo retval;
  retval.x = LoadCheckDouble(L, "x");
  retval.y = LoadCheckDouble(L, "y");
  retval.name = LoadString(L, "name");
  retval.factories = LoadCheckInteger(L, "fact");
  retval.population = LoadCheckInteger(L, "pop");
  // TODO:  Make sure this uses "move" semantics
  return retval;
}