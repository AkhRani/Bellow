#include "product.h"
#include "gtest/gtest.h"

TEST(ProductTest, Creation) {
  Product prod;
  EXPECT_EQ(0., prod.GetGrowthRate());
  EXPECT_EQ(0., prod.GetCost());
  EXPECT_EQ(0, prod.GetAmount());
  EXPECT_EQ(0, prod.GetMax());

  Product prod2(15);
  EXPECT_EQ(0., prod2.GetGrowthRate());
  EXPECT_EQ(0., prod2.GetCost());
  EXPECT_EQ(0, prod2.GetAmount());
  EXPECT_EQ(15, prod2.GetMax());

  Product prod3(1., 10., 5, 20);
  EXPECT_EQ(1., prod3.GetGrowthRate());
  EXPECT_EQ(10., prod3.GetCost());
  EXPECT_EQ(5, prod3.GetAmount());
  EXPECT_EQ(20, prod3.GetMax());
}

TEST(ProductTest, Manipulation) {
  Product prod;
  prod.SetGrowthRate(2.2);
  EXPECT_EQ(2.2, prod.GetGrowthRate());

  prod.SetCost(1.5);
  EXPECT_EQ(1.5, prod.GetCost());

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
  Product prod(0., 10., 5, 20);
  prod.Grow(0.);
  EXPECT_EQ(5, prod.GetAmount());

  prod.Grow(10.);
  EXPECT_EQ(6, prod.GetAmount());

  prod.Grow(15.);
  EXPECT_EQ(7, prod.GetAmount());

  prod.Grow(15.);
  EXPECT_EQ(9, prod.GetAmount());

  prod.Grow(110.);
  EXPECT_EQ(20, prod.GetAmount());

  prod.Grow(10.);
  EXPECT_EQ(20, prod.GetAmount());

  Product prod2(.2, 10., 5, 20);
  prod2.Grow(0.);
  EXPECT_EQ(6, prod2.GetAmount());

  prod2.Grow(10.);
  EXPECT_EQ(8, prod2.GetAmount());
}
