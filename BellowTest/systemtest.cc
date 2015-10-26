/** System-level tests.
 *
 * For now, all system tests are in a single file,
 * but this may change in the future.
 */
#include <memory>
#include <string>

#include "fleet.h"
#include "game.h"
#include "lua_api.h"
#include "player.h"
#include "system_info.h"
#include "testutils.h"

#include "gtest/gtest.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::unique_ptr;

class SystemTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_L = luaL_newstate();
    lua_gc(m_L, LUA_GCSTOP, 0);
    luaL_openlibs(m_L);
    lua_gc(m_L, LUA_GCRESTART, 0);

    EXPECT_EQ(false, luaL_dofile(m_L, "Scripts/GameGenerator.lua"));
    EXPECT_EQ(false, luaL_dofile(m_L, "testutils.lua"));
    ASSERT_EQ(false, luaL_dostring(m_L, "math.randomseed(1234)"));

    lua_getglobal(m_L, "NewGame");
    EXPECT_EQ(true, lua_isfunction(m_L, -1));
    EXPECT_EQ(0, lua_pcall(m_L, 0, 1, -1));
    m_pGame =  unique_ptr<Game> (new Game(m_L));
    RegisterApi(m_L, m_pGame.get());
  }

  virtual void TearDown() {
    lua_close(m_L);
  }

  lua_State *m_L;
  unique_ptr<Game> m_pGame;
};

TEST_F(SystemTest, Creation) {
  // Default parameters for now
  EXPECT_EQ(48, m_pGame->GetSystemCount());
  EXPECT_EQ(1, m_pGame->GetPlayerCount());
  EXPECT_EQ(30., m_pGame->GetGalaxySize());

  EXPECT_EQ(false, luaL_dofile(m_L, "systemtest_creation.lua"));
  EXPECT_EQ(string(""), CheckLuaFailures(m_L));
  // TODO:  Compare loaded game to saved game
  // std::string serialized;
}

TEST_F(SystemTest, Exploration) {
  EXPECT_EQ(false, luaL_dofile(m_L, "systemtest_exploration.lua"));
  EXPECT_EQ(string(""), CheckLuaFailures(m_L));
}