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
    ,m_Planet(game, L, "planet")
    ,m_ID(id) // Not sure whether to do this, or add id parameter to visitor class
{
  lua_pop(L, 1);
}


StarSystem::StarSystem(IGame& game, int id, double x, double y) :
  m_Name("dummy") 
  , m_X(x)
  , m_Y(y)
  , m_ID(id)
  , m_Planet(game, 100)
{
}


void StarSystem::Save(string& serialized) {
  serialized.append("\n{ name = \"");
  serialized.append(m_Name);
  serialized.append("\", x = " + std::to_string(m_X));
  serialized.append(", y = " + std::to_string(m_Y));
  serialized.append(", planet =");
  m_Planet.Save(serialized);
  serialized.append(" }");
}


void StarSystem::FinishLoad() {
  m_Planet.FinishLoad();
}


// Note:  Does this need to be a pointer?
Planet& StarSystem::GetPlanet() {
  return m_Planet;
}


void StarSystem::NextTurn() {
  m_Planet.Update();
}