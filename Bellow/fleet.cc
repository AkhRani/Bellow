#include <assert.h>
#include <math.h>

#include "fleet.h"
#include "galaxy.h"
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

Fleet::Fleet(Player& owner, IGalaxy& galaxy, int systemId) :
  m_Owner(owner)
  , m_Galaxy(galaxy)
  , m_State(ST_ORBITING)
  , m_Position(0., 0.)
  , m_Orbit(systemId)
  , m_Target(-1)
{
  StarSystem* pSystem = m_Galaxy.GetStarSystem(systemId);
  assert(pSystem);
  if (!pSystem) {
    throw(std::runtime_error("Bad system ID passed to Fleet Constructor"));
  }
  // TODO: m_Position = pSystem->GetPosition or pSystem->GetPosition(m_Position)
  m_Position = pSystem->m_Position;
}


Fleet::Fleet(Player& owner, IGalaxy& galaxy, lua_State *L) :
  m_Owner(owner)
  , m_Galaxy(galaxy)
  , m_State(ST_ORBITING)
  , m_Position(L)
  , m_Orbit(LoadOptInteger(L, "orbit", -1))
  , m_Target(LoadOptInteger(L, "target", -1))
{
  int savedState = LoadOptInteger(L, "state", ST_ORBITING);
  if (savedState >= ST_ORBITING && savedState <= ST_ARRIVING) {
    m_State = static_cast<FleetState>(savedState);
  }
  lua_pop(L, 1);
  // TODO: else issue warning, "Defaulting to Orbiting"
  // Sanity checks
  if (m_Target >= 0 && m_State == ST_ORBITING ||
    m_Orbit >= 0 && (m_State != ST_ORBITING && m_State != ST_LAUNCHING)) {
    throw(std::runtime_error("load error:  Bad fleet state"));
  }
  // TODO:  If orbiting, double-check or set position
}


void Fleet::Save(string &rep) {
  rep.append("{ state=");
  rep.append(to_string(m_State));
  rep.append(", ");
  m_Position.Save(rep);
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
  assert(m_Galaxy.GetStarSystem(system) != nullptr);
  if (IsInOrbit() || IsLaunching()) {
    if (system == m_Orbit) {
      // Cancel launch
      m_State = ST_ORBITING;
      m_Target = -1;
    }
    else {
      m_State = ST_LAUNCHING;
      m_Target = system;
    }
  }
}


/**! First stage of fleet movement.
 * Normally called after construction.
 */
void Fleet::Move() {
  if (ST_TRAVELING == m_State || ST_LAUNCHING == m_State) {
    assert(m_Target >= 0);
    // TODO:  Fleet speed
    if (m_Galaxy.Move(m_Position, 1.0, m_Target)) {
      // TODO (merging):  Let the destination system know we're in the vicinity
      m_State = ST_ARRIVING;
    }
    else {
      m_State = ST_TRAVELING;
      m_Orbit = -1;
    }
  }
}


/**! Transition from approaching to arriving.
 * Normally called by the Game class after successful combat,
 * or if no combat occurs.
 */
void Fleet::Arrive() {
  if (ST_ARRIVING == m_State) {
    m_Owner.Explore(m_Target);
    m_State = ST_ORBITING;
    m_Orbit = m_Target;
    m_Target = 0;
  }
}

void Fleet::GetPosition(double &x, double &y) {
  x = m_Position.x;
  y = m_Position.y;
}