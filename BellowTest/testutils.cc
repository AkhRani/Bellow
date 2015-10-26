#include "testutils.h"
#include "gtest/gtest.h"

#include "player.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void RunLua(lua_State *L, const char* script) {
  ASSERT_EQ(false, luaL_dostring(L, script));
  ASSERT_EQ(true, lua_istable(L, -1));
}

const char* CheckLuaFailures(lua_State *L) {
  lua_getglobal(L, "g_Failures");
  EXPECT_EQ(1, lua_isstring(L, -1));
  const char *retval = lua_tostring(L, -1);
  lua_pop(L, 1);
  return retval;
}