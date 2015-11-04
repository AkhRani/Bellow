#include <assert.h>
#include <math.h>

#include "fleet.h"
#include "player.h"
#include "star_system.h"
#include "util.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::to_string;

Fleet::Fleet(Player& owner, IStarSystemOwner& systemOwner, int systemId) :
  m_Owner(owner)
  , m_SystemOwner(systemOwner)
  , m_State(ST_ORBITING)
  , m_X(0.)
  , m_Y(0.)
  , m_Orbit(systemId)
  , m_Target(0)
{
  StarSystem* pSystem = systemOwner.GetStarSystem(systemId);
  assert(pSystem);
  if (!pSystem) {
    throw(std::runtime_error("Bad system ID passed to Fleet Constructor"));
  }
  m_X = pSystem->m_X;
  m_Y = pSystem->m_Y;
}


Fleet::Fleet(Player& owner, IStarSystemOwner& systemOwner, lua_State *L) :
  m_Owner(owner)
  , m_SystemOwner(systemOwner)
  , m_State(ST_ORBITING)
  , m_X(LoadCheckDouble(L, "x"))
  , m_Y(LoadCheckDouble(L, "y"))
  , m_Orbit(LoadOptInteger(L, "orbit", 0))
  , m_Target(LoadOptInteger(L, "target", 0))
{
  int savedState = LoadOptInteger(L, "state", ST_ORBITING);
  if (savedState >= ST_ORBITING && savedState <= ST_ARRIVING) {
    m_State = static_cast<FleetState>(savedState);
  }
  lua_pop(L, 1);
  // TODO: else issue warning, "Defaulting to Orbiting"
  // Sanity checks
  if (m_Target && m_State == ST_ORBITING ||
    m_Orbit && (m_State != ST_ORBITING && m_State != ST_LAUNCHING)) {
    throw(std::runtime_error("load error:  Bad fleet state"));
  }
}


void Fleet::Save(string &rep) {
  rep.append("{ state=");
  rep.append(to_string(m_State));
  rep.append(", x=");
  rep.append(to_string(m_X));
  rep.append(", y=");
  rep.append(to_string(m_Y));
  if (IsInOrbit() || IsLaunching()) {
    rep.append(", orbit=");
    rep.append(to_string(m_Orbit));
  }
  if (!IsInOrbit()) {
    rep.append(", target=");
    rep.append(to_string(m_Target));
  }
  rep.append("}");
}


void Fleet::SetDestination(int system) {
  assert(m_SystemOwner.GetStarSystem(system) != nullptr);
  if (IsInOrbit() || IsLaunching()) {
    if (system == m_Orbit) {
      // Cancel launch
      m_State = ST_ORBITING;
      m_Target = 0;
    }
    else {
      m_State = ST_LAUNCHING;
      m_Target = system;
    }
  }
}


void Fleet::Move() {
  // TODO:  Fleet speed
  double speed = 1.0;

  if (ST_TRAVELING == m_State || ST_LAUNCHING == m_State) {
    assert(m_Target != 0);
    StarSystem *pTarget = m_SystemOwner.GetStarSystem(m_Target);
    double dx = pTarget->m_X - m_X;
    double dy = pTarget->m_Y - m_Y;
    double distance = sqrt(dx * dx + dy * dy);
    if (distance <= speed) {
      m_X = pTarget->m_X;
      m_Y = pTarget->m_Y;
      // TODO (merging):  Let the destination system know we're in the vicinity
      m_State = ST_ARRIVING;
    }
    else {
      double angle = atan2(dy, dx);
      m_X += speed * cos(angle);
      m_Y += speed * sin(angle);
      m_State = ST_TRAVELING;
      m_Orbit = 0;
    }
  }
}


//! Transition from approaching to arriving.
// Normally called by the Game class after successful combat,
// or if no combat occurs.
void Fleet::Arrive() {
  if (ST_ARRIVING == m_State) {
    m_Owner.Explore(m_Target);
    m_State = ST_ORBITING;
    m_Orbit = m_Target;
    m_Target = 0;
  }
}

void Fleet::GetPosition(double &x, double &y) {
  x = m_X;
  y = m_Y;
}