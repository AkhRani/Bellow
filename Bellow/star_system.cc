#include "star_system.h"
#include "game.h"
#include "planet.h"
#include "util.h"

extern "C" {
#include "lua.h"
}

StarSystem::StarSystem(const Game& game, lua_State *L) :
    m_Name(LoadString(L, "name")),
    m_X(LoadCheckDouble(L, "x")),
    m_Y(LoadCheckDouble(L, "y")) {
  lua_getfield(L, -1, "planet");
  if (lua_istable(L, -1)) {
    m_Planet.reset(new Planet(game, L));
  }
  else {
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}