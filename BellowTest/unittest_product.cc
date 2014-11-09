#include "product.h"
#include "gtest/gtest.h"

TEST(ProductTest, Creation) {
  Product prod;
  EXPECT_EQ(0., prod.getGrowthRate());
  EXPECT_EQ(0., prod.getCost());
  EXPECT_EQ(0, prod.getAmount());
  EXPECT_EQ(0, prod.getMax());

  Product prod2(15);
  EXPECT_EQ(0., prod2.getGrowthRate());
  EXPECT_EQ(0., prod2.getCost());
  EXPECT_EQ(0, prod2.getAmount());
  EXPECT_EQ(15, prod2.getMax());

  Product prod3(1., 10., 5, 20);
  EXPECT_EQ(1., prod3.getGrowthRate());
  EXPECT_EQ(10., prod3.getCost());
  EXPECT_EQ(5, prod3.getAmount());
  EXPECT_EQ(20, prod3.getMax());
}

TEST(ProductTest, Manipulation) {
  Product prod;
  prod.setGrowthRate(2.2);
  EXPECT_EQ(2.2, prod.getGrowthRate());

  prod.setCost(1.5);
  EXPECT_EQ(1.5, prod.getCost());

  // Respect maximum
  prod.setAmount(10);
  EXPECT_EQ(0, prod.getAmount());

  prod.setMax(5);
  prod.setAmount(10);
  EXPECT_EQ(5, prod.getMax());
  EXPECT_EQ(5, prod.getAmount());

  prod.setMax(4);
  EXPECT_EQ(4, prod.getMax());
  EXPECT_EQ(4, prod.getAmount());
}

TEST(ProductTest, Growth) {
  Product prod(0., 10., 5, 20);
  prod.grow(0.);
  EXPECT_EQ(5, prod.getAmount());

  prod.grow(10.);
  EXPECT_EQ(6, prod.getAmount());

  prod.grow(15.);
  EXPECT_EQ(7, prod.getAmount());

  prod.grow(15.);
  EXPECT_EQ(9, prod.getAmount());

  prod.grow(110.);
  EXPECT_EQ(20, prod.getAmount());

  prod.grow(10.);
  EXPECT_EQ(20, prod.getAmount());

  Product prod2(.2, 10., 5, 20);
  prod2.grow(0.);
  EXPECT_EQ(6, prod2.getAmount());

  prod2.grow(10.);
  EXPECT_EQ(8, prod2.getAmount());
}
