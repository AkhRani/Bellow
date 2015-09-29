#include "fleet.h"
#include "game.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::to_string;

Fleet::Fleet(const Player& owner, double x, double y) :
  m_Owner(owner)
  , m_X(x)
  , m_Y(y)
  , m_DestX(x)
  , m_DestY(y)
{
}

Fleet::Fleet(const Player& owner, lua_State *L) :
  m_X(LoadCheckDouble(L, "x"))
  , m_Y(LoadCheckDouble(L, "y"))
  , m_DestX(LoadOptDouble(L, "dstx", m_X))
  , m_DestY(LoadOptDouble(L, "dsty", m_Y))
  , m_Owner(owner)
{
  lua_pop(L, 1);
}

void Fleet::Save(string &rep) {
  rep.append("{ x=");
  rep.append(to_string(m_X));
  rep.append(", y=");
  rep.append(to_string(m_Y));
  rep.append(", dstx=");
  rep.append(to_string(m_DestX));
  rep.append(", dsty=");
  rep.append(to_string(m_DestY));
  rep.append("}");
}


void Fleet::SetDestination(StarSystem& system) {
}
