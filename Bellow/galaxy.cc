#include "galaxy.h"
#include "system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

Galaxy::Galaxy() {
}

Galaxy::Galaxy(IGame& game, lua_State *L, const char *field) {
  if (field) {
    lua_getfield(L, -1, field);
  }

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
          StarSystem sys(game, L, idx);
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

  if (field) {
    lua_pop(L, 1);
  }
}

int Galaxy::VisitSystems(SystemVisitor &visitor) {
  int retval(0);
  for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it) {
    retval += visitor(*it);
  }
  return retval;
}

bool Galaxy::GetSystemInfo(unsigned int id, SystemInfo& info) {
  // TODO:  Player-based
  if (id < m_Systems.size()) {
    StarSystem &system(m_Systems[id]);
    info.x = system.m_X;
    info.y = system.m_Y;
    info.name = system.m_Name;
    return true;
  }
  return false;
}
