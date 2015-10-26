#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <string>

extern "C" {
  struct lua_State;
}

void RunLua(lua_State *L, const char* script);
const char* CheckLuaFailures(lua_State *L);

#endif