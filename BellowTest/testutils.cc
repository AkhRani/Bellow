#include "testutils.h"
#include "gtest/gtest.h"

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
