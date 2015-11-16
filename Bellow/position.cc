#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "position.h"
#include "util.h"

using std::string;
using std::to_string;

Position::Position(lua_State *L) {
  lua_getfield(L, -1, "pos");
  x = LoadCheckDouble(L, "x");
  y = LoadCheckDouble(L, "y");
  lua_pop(L, 1);
}

void Position::Save(string &rep) {
  rep.append("pos = { x=");
  rep.append(to_string(x));
  rep.append(", y=");
  rep.append(to_string(y));
  rep.append("}");
}