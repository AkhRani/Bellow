#include <stdexcept>

#include "product.h"
#include "gtest/gtest.h"
#include "testutils.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using std::string;
using std::runtime_error;

TEST(ProductTest, Creation) {
  Product prod;
  EXPECT_EQ(0, prod.GetAmount());
  EXPECT_EQ(0, prod.GetMax());

  Product prod2(15);
  EXPECT_EQ(0, prod2.GetAmount());
  EXPECT_EQ(15, prod2.GetMax());

  Product prod3(5, 20);
  EXPECT_EQ(5, prod3.GetAmount());
  EXPECT_EQ(20, prod3.GetMax());
}

TEST(ProductTest, Load) {
  lua_State *L = luaL_newstate();
  Product prod;

  // Nominal
  RunLua(L, "return { amount = 10, invested = 4 }");
  prod.Load(L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(10, prod.GetAmount());
  EXPECT_LE(prod.GetMax(), prod.GetAmount());
  EXPECT_EQ(4, prod.GetInvested());

  // Off-nominal
  RunLua(L, "return { amount = 10.3, invested = 04.24 }");
  prod.Load(L);
  EXPECT_EQ(0, lua_gettop(L));
  EXPECT_EQ(10, prod.GetAmount());
  EXPECT_LE(prod.GetMax(), prod.GetAmount());
  EXPECT_EQ(4, prod.GetInvested());

  // Errors
  RunLua(L, "return { invested = 4 }");
  EXPECT_THROW(prod.Load(L), runtime_error);

  RunLua(L, "return { amount = 11 }");
  EXPECT_THROW(prod.Load(L), runtime_error);

}

TEST(ProductTest, Save) {
  lua_State *L = luaL_newstate();
  Product prod(100);
  prod.SetAmount(50);
  string serialized("return ");
  prod.Save(serialized);

  Product prod2;
  RunLua(L, serialized.c_str());
  prod2.Load(L);
  EXPECT_EQ(50, prod2.GetMax());
  EXPECT_EQ(50, prod2.GetAmount());
}

TEST(ProductTest, Manipulation) {
  Product prod;

  // Respect maximum
  prod.SetAmount(10);
  EXPECT_EQ(0, prod.GetAmount());

  prod.SetMax(5);
  prod.SetAmount(10);
  EXPECT_EQ(5, prod.GetMax());
  EXPECT_EQ(5, prod.GetAmount());

  prod.SetMax(4);
  EXPECT_EQ(4, prod.GetMax());
  EXPECT_EQ(4, prod.GetAmount());
}

TEST(ProductTest, Growth) {
  Product prod(5, 20);
  prod.Grow(10, 0);
  EXPECT_EQ(5, prod.GetAmount());

  prod.Grow(10, 10);
  EXPECT_EQ(6, prod.GetAmount());

  prod.Grow(10, 15);
  EXPECT_EQ(7, prod.GetAmount());

  prod.Grow(10, 15);
  EXPECT_EQ(9, prod.GetAmount());

  prod.Grow(10, 110);
  EXPECT_EQ(20, prod.GetAmount());

  prod.Grow(10, 10);
  EXPECT_EQ(20, prod.GetAmount());

  /*
  TODO:  Move to population test.
  
  Product prod2(.2, 10., 5, 20);
  prod2.Grow(0.);
  EXPECT_EQ(6, prod2.GetAmount());

  prod2.Grow(10.);
  EXPECT_EQ(8, prod2.GetAmount());
  */
}
