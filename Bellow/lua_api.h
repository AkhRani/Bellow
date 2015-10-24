#ifndef LUA_API_H
#define LUA_API_H

struct lua_State;
class Game;

bool RegisterApi(lua_State *L, Game *pGame);

#endif