#include <math.h>

#include "fleet.h"
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
  , m_Orbiting(true)
{
}


Fleet::Fleet(const Player& owner, lua_State *L) :
  m_Owner(owner)
  , m_X(LoadCheckDouble(L, "x"))
  , m_Y(LoadCheckDouble(L, "y"))
  , m_DestX(LoadOptDouble(L, "dstx", m_X))
  , m_DestY(LoadOptDouble(L, "dsty", m_Y))
  , m_Orbiting(m_X == m_DestX && m_Y == m_DestY)
{
  lua_pop(L, 1);
}


void Fleet::Save(string &rep) {
  rep.append("{ x=");
  rep.append(to_string(m_X));
  rep.append(", y=");
  rep.append(to_string(m_Y));
  if (!m_Orbiting) {
    rep.append(", dstx=");
    rep.append(to_string(m_DestX));
    rep.append(", dsty=");
    rep.append(to_string(m_DestY));
  }
  rep.append("}");
}


void Fleet::SetDestination(double x, double y) {
  m_DestX = x;
  m_DestY = y;
}


void Fleet::Move() {
  // TODO:  Fleet speed
  double speed = 1.0;

  if (m_DestX != m_X || m_DestY != m_Y) {
    double dx = m_DestX - m_X;
    double dy = m_DestY - m_Y;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance <= speed) {
      m_X = m_DestX;
      m_Y = m_DestY;
      m_Orbiting = true;
    }
    else {
      double angle = atan2(dy, dx);
      m_X += speed * cos(angle);
      m_Y += speed * sin(angle);
      m_Orbiting = false;
    }
  }
}

void Fleet::GetPosition(double &x, double &y) {
  x = m_X;
  y = m_Y;
}