#include "player_system_info.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::to_string;

PlayerSystemInfo::PlayerSystemInfo(const string& name, uint32_t fact, uint32_t pop) :
  name(name)
  , factories(fact)
  , population(pop)
{
  
}

PlayerSystemInfo::PlayerSystemInfo() :
  name("?")
  , factories(0)
  , population(0)
{
  
}

PlayerSystemInfo PlayerSystemInfo::Load(lua_State *L) {
  PlayerSystemInfo retval;
  retval.name = LoadString(L, "name");
  int fact = LoadOptInteger(L, "fact", 0);
  int pop = LoadOptInteger(L, "pop", 0);
  lua_pop(L, 1);

  // Sanity check
  if (fact < 0 || pop < 0) {
    throw(std::runtime_error("load error:  Bad system info"));
  }
  retval.factories = fact;
  retval.population = pop;
  // This should use "move" semantics, not that it matters
  return retval;
}

void PlayerSystemInfo::Save(string &rep) {
  rep.append("{ name=\"");
  rep.append(name);
  rep.append("\"");
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
