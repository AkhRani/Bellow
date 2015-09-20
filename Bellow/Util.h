#ifndef UTIL_H
#define UTIL_H

extern "C" {
  struct lua_State;
}

#include <stdexcept>
#include <functional>

// TODO:  subclass runtime error to store failure context
static inline void LoadCheck(bool cond) {
  if (!cond) throw(std::runtime_error("load error"));
}

const char* LoadString(lua_State *L, const char *name);
int LoadCheckInteger(lua_State *L, const char *name);
double LoadCheckDouble(lua_State *L, const char *name);

#endif