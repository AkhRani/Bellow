#include "galaxy.h"
#include "system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::bind;
using std::string;
using std::vector;
using namespace std::placeholders;

void Galaxy::LoadSystem(lua_State *L, int idx) {
  StarSystem sys(m_Game, L, idx);
  if (sys.m_X >= 0. && sys.m_X <= m_Size &&
    sys.m_Y >= 0. && sys.m_Y <= m_Size) {
    m_Systems.push_back(sys);
  }
  else {
    // TODO:  Load warning / error
  }
}


Galaxy::Galaxy(IGame& game, lua_State *L, const char *field) :
    m_Game(game) {
  if (field) {
    lua_getfield(L, -1, field);
  }

  // Top of Lua stack should be a table of { size, systems }
  if (lua_istable(L, -1)) {
    m_Size = LoadCheckDouble(L, "size");
    LoadTableOfTables(L, "systems", bind(&Galaxy::LoadSystem, this, _1, _2));
  }
  lua_pop(L, 1);

  if (field) {
    lua_pop(L, 1);
  }
}


void Galaxy::Save(string& rep) {
  rep.append("\n{ size = ");
  rep.append(std::to_string(m_Size));
  rep.append(", systems = {");
  for (auto v : m_Systems) {
    v.Save(rep);
    rep.append(", ");
  }
  rep.append("\n} }");
}


void Galaxy::FinishLoad() {
  for (auto& system : m_Systems) {
    system.FinishLoad();
  }
}


int Galaxy::VisitSystems(SystemVisitor &visitor) {
  int retval(0);
  for (auto& system : m_Systems) {
    retval += visitor(system);
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

StarSystem* Galaxy::GetStarSystem(int systemId) {
  if (systemId > 0 && size_t(systemId) <= m_Systems.size()) {
    return &m_Systems[systemId - 1];
  }
  return nullptr;
}