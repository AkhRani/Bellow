extern "C" {
  struct lua_State;
}

void RunLua(lua_State *L, const char *script);
