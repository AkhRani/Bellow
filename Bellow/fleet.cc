#include "fleet.h"
#include "game.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

Fleet::Fleet(const Player& owner, lua_State *L) :
  m_X(LoadCheckDouble(L, "x"))
  , m_Y(LoadCheckDouble(L, "y"))
  , m_Owner(owner)
{
  lua_pop(L, 1);
}

Fleet::Fleet(const Player& owner, double x, double y) :
  m_Owner(owner)
  , m_X(x)
  , m_Y(y)
{
}

void Fleet::SetDestination(StarSystem& system) {
}
