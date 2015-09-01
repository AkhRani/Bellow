// LuaConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int _tmain(int argc, _TCHAR* argv[])
{
  lua_State *L = luaL_newstate();
  char buff[100];
  printf("> ");
  while (fgets(buff, 100, stdin)) {
    if (buff[0] == 'q') {
      break;
    }
    bool error = luaL_dostring(L, buff);
    if (error) {
      printf("error: %s\n> ", lua_tostring(L, -1));
    }
    else {
      if (lua_isnumber(L, -1)) {
        printf("%lf\n> ", lua_tonumber(L, -1));
      }
      else {
        printf("OK\n> ");
      }
    }
  }
  return 0;
}

