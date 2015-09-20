#include "star_system.h"
#include "game.h"
#include "planet.h"
#include "util.h"

extern "C" {
#include "lua.h"
}

using namespace std;

StarSystem::StarSystem(const IGame& game, lua_State *L, int id) :
    m_Name(LoadString(L, "name"))
    ,m_X(LoadCheckDouble(L, "x"))
    ,m_Y(LoadCheckDouble(L, "y"))
    ,m_ID(id) // Not sure whether to do this, or add id parameter to visitor class
{
  lua_getfield(L, -1, "planet");
  if (lua_istable(L, -1)) {
    m_Planet.reset(new Planet(game, L));
  }
  else {
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

// Note:  Does this need to be a pointer?
weak_ptr<Planet> StarSystem::GetPlanet() {
  return m_Planet;
}