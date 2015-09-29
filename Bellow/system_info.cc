#include "system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::to_string;

SystemInfo SystemInfo::Load(lua_State *L) {
  SystemInfo retval;
  retval.x = LoadCheckDouble(L, "x");
  retval.y = LoadCheckDouble(L, "y");
  retval.name = LoadString(L, "name");
  retval.factories = LoadOptInteger(L, "fact", 0);
  retval.population = LoadOptInteger(L, "pop", 0);
  lua_pop(L, 1);
  // TODO:  Make sure this uses "move" semantics
  return retval;
}

void SystemInfo::Save(string &rep) {
  rep.append("{ name=\"");
  rep.append(name);
  rep.append("\", x=");
  rep.append(to_string(x));
  rep.append(", y=");
  rep.append(to_string(y));
  if (factories) {
    rep.append(", fact=");
    rep.append(to_string(factories));
  }
  if (population) {
    rep.append(", pop=");
    rep.append(to_string(population));
  }
  rep.append("}");
}