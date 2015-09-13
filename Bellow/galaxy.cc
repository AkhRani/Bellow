#include "galaxy.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

Galaxy::Galaxy() {
}

Galaxy::Galaxy(IGame& game, lua_State *L) {
  // Top of Lua stack should be a table of { size, systems }
  if (lua_istable(L, -1)) {
    m_Size = LoadCheckDouble(L, "size");

    lua_getfield(L, -1, "systems");
    if (lua_istable(L, -1)) {
      int idx = 1;
      while (1) {
        int top = lua_gettop(L);
        lua_rawgeti(L, -1, idx);
        if (lua_istable(L, -1)) {
          StarSystem sys(game, L);
          if (sys.m_X >= 0. && sys.m_X <= m_Size &&
            sys.m_Y >= 0. && sys.m_Y <= m_Size) {
            m_Systems.push_back(sys);
          }
          else {
            // TODO:  Load warning / error
          }
          idx++;
        }
        else {
          lua_pop(L, 1);
          break;
        }
      }
    }
    lua_pop(L, 1);
  }
}

int Galaxy::VisitPlanets(SystemVisitor &visitor) {
  int retval(0);
  for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it) {
    retval += visitor(*it);
  }
  return retval;
}
