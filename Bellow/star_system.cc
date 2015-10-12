#include "star_system.h"
#include "game.h"
#include "planet.h"
#include "util.h"

extern "C" {
#include "lua.h"
}

using std::string;
using std::weak_ptr;

StarSystem::StarSystem(IGame& game, lua_State *L, int id) :
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

void StarSystem::Save(string& serialized) {
  serialized.append("\n{ name = \"");
  serialized.append(m_Name);
  serialized.append("\", x = " + std::to_string(m_X));
  serialized.append(", y = " + std::to_string(m_Y));
  if (m_Planet) {
    serialized.append(", planet =");
    m_Planet->Save(serialized);
  }
  serialized.append(" }");
}


void StarSystem::FinishLoad() {
  if (m_Planet) {
    m_Planet->FinishLoad();
  }
}


// Note:  Does this need to be a pointer?
weak_ptr<Planet> StarSystem::GetPlanet() {
  return m_Planet;
}


void StarSystem::NextTurn() {
  if (m_Planet) {
    m_Planet->Update();
  }
}