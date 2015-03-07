#include "testutils.h"
#include "gtest/gtest.h"

#include "player.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
// TODO:  Move to testutils.cc
void RunLua(lua_State *L, const char *script) {
  ASSERT_EQ(false, luaL_dostring(L, script));
  ASSERT_EQ(true, lua_istable(L, -1));
}

MockGame::MockGame() : m_Player(new Player("human")) {};
std::weak_ptr<Player> MockGame::GetPlayer(const std::string &name) const { return m_Player; }

