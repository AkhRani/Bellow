#include "galaxy.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

Galaxy::Galaxy() {
}

Galaxy::Galaxy(Game& game, lua_State *L) {
  // Top of Lua stack should be a table of systems
  if (lua_istable(L, -1)) {
    int idx = 1;
    while (1) {
      int top = lua_gettop(L);
      lua_rawgeti(L, -1, idx);
      if (lua_istable(L, -1)) {
        m_Systems.push_back(StarSystem(game, L));
        idx++;
      }
      else {
        lua_pop(L, 1);
        break;
      }
    }
  }
}

int Galaxy::VisitPlanets(SystemVisitor &visitor) {
  int retval(0);
  for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it) {
    retval += visitor(*it);
  }
  return retval;
}
