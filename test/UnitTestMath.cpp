#include <gtest/gtest.h>
#include "../include/Global.h"
#include "ApeironTest.h"

#ifdef DEBUG_MODE

namespace aprn {

/***************************************************************************************************************************************************************
* Sequences and Series
***************************************************************************************************************************************************************/
TEST_F(ApeironTest, GetInteger)
{
  constexpr size_t size = 100;
  FOR(i, size) EXPECT_EQ(GetInteger(std::make_integer_sequence<int, size>{}, i), i);
}

/***************************************************************************************************************************************************************
* General Arithmetic Functions
***************************************************************************************************************************************************************/
TEST_F(ApeironTest, Sum)
{
  constexpr int intSum = Sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(intSum, 55);

  constexpr Float floatSum = Sum(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0);
  EXPECT_DOUBLE_EQ(floatSum, 55.0);
}

TEST_F(ApeironTest, Product)
{
  constexpr int intProd = Product(1, 2, 3, 4, 5);
  EXPECT_EQ(intProd, 120);

  constexpr Float floatProd = Product(1.0, 2.0, 3.0, 4.0, 5.0);
  EXPECT_DOUBLE_EQ(floatProd, 120.0);
}

TEST_F(ApeironTest, Divide)
{
  EXPECT_EQ(Divide(20, 5), Four);
  EXPECT_THROW(Divide(1, 0), std::logic_error);

  EXPECT_EQ(Divide(20.0, 5.0), Four);
  EXPECT_THROW(Divide(1.0, 0.0), std::logic_error);
}

TEST_F(ApeironTest, Modulo)
{
  EXPECT_EQ(Modulo(20, 5), 0);
  EXPECT_EQ(Modulo(20, 3), 2);
  EXPECT_THROW(Modulo(1, 0), std::logic_error);

  EXPECT_DOUBLE_EQ(Modulo(20.0, 5.0), Zero);
  EXPECT_DOUBLE_EQ(Modulo(20.0, 3.0), Two);
  EXPECT_DOUBLE_EQ(Modulo(20.1234, 3.0), 2.1234);
  EXPECT_THROW(Modulo(1.0, 0.0), std::logic_error);
}

/***************************************************************************************************************************************************************
* Combinatorial Functions
***************************************************************************************************************************************************************/
TEST_F(ApeironTest, Factorial)
{
  EXPECT_EQ(Factorial(0), 1);
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(5), 120);
}

TEST_F(ApeironTest, FactorialQuotient)
{
  EXPECT_EQ(FactorialQuotient(1, 0), 1);
  EXPECT_EQ(FactorialQuotient(2, 1), 2);
  EXPECT_EQ(FactorialQuotient(2, 2), 1);
  EXPECT_EQ(FactorialQuotient(5, 3), 20);
  EXPECT_THROW(FactorialQuotient(2, 3), std::logic_error);
}

TEST_F(ApeironTest, Choose)
{
  EXPECT_EQ(Choose(3, 0), 1);
  EXPECT_EQ(Choose(3, 1), 3);
  EXPECT_EQ(Choose(3, 2), 3);
  EXPECT_EQ(Choose(3, 3), 1);
  EXPECT_THROW(Choose(3, 4), std::logic_error);
}

/***************************************************************************************************************************************************************
* Power Functions
***************************************************************************************************************************************************************/
TEST_F(ApeironTest, iPow)
{
  EXPECT_EQ(iPow(2.0, 1), 2.0);
  EXPECT_EQ(iPow(2.0, 2), 4.0);
  EXPECT_EQ(iPow(2.0, 3), 8.0);
  EXPECT_THROW(iPow(2.0, 31), std::logic_error);
}

TEST_F(ApeironTest, Square)
{
  EXPECT_EQ(Square(2.0), 4.0);
  EXPECT_EQ(Square(3.0), 9.0);
  EXPECT_EQ(Square(4.0), 16.0);
}

TEST_F(ApeironTest, Cube)
{
  EXPECT_EQ(Cube(2.0), 8.0);
  EXPECT_EQ(Cube(3.0), 27.0);
  EXPECT_EQ(Cube(4.0), 64.0);
}

TEST_F(ApeironTest, Sqrt)
{
  EXPECT_EQ(Sqrt(4.0), 2.0);
  EXPECT_EQ(Sqrt(9.0), 3.0);
  EXPECT_EQ(Sqrt(16.0), 4.0);
  EXPECT_TRUE(isNaN(Sqrt(-1.0)));
  EXPECT_TRUE(isNaN(Sqrt(InfFloat<>)));
}

TEST_F(ApeironTest, Cbrt)
{
  EXPECT_EQ(Cbrt(8.0), 2.0);
  EXPECT_EQ(Cbrt(27.0), 3.0);
  EXPECT_EQ(Cbrt(-8.0), -2.0);
  EXPECT_EQ(Cbrt(-27.0), -3.0);
}

TEST_F(ApeironTest, Hypot)
{
  EXPECT_EQ(Hypot(3.0, 4.0), 5.0);
  EXPECT_EQ(Hypot(5.0, 12.0), 13.0);
  EXPECT_EQ(Hypot(8.0, 15.0), 17.0);
  EXPECT_EQ(Hypot(7.0, 24.0), 25.0);
}

TEST_F(ApeironTest, Exp)
{
  EXPECT_DOUBLE_EQ(Exp(0.0), 1.0);
  EXPECT_DOUBLE_EQ(Exp(1.0), e);
  EXPECT_DOUBLE_EQ(Exp(2.0), iPow(e, 2));
  EXPECT_DOUBLE_EQ(Exp(3.0), iPow(e, 3));
}

TEST_F(ApeironTest, Sin)
{
  EXPECT_DOUBLE_EQ(Sin(Zero), Zero);
  EXPECT_DOUBLE_EQ(Sin(HalfPi), One);
  EXPECT_DOUBLE_EQ(Sin(SixthPi), Half);
  EXPECT_DOUBLE_EQ(Sin(QuartPi), One/Sqrt(Two));
  EXPECT_DOUBLE_EQ(Sin(ThirdPi), Half*Sqrt(Three));
}

TEST_F(ApeironTest, Cos)
{
  EXPECT_DOUBLE_EQ(Cos(Zero), One);
  EXPECT_NEAR(Cos(HalfPi), Zero, Small);
  EXPECT_DOUBLE_EQ(Cos(SixthPi), Half*Sqrt(Three));
  EXPECT_DOUBLE_EQ(Cos(QuartPi), One/Sqrt(Two));
  EXPECT_DOUBLE_EQ(Cos(ThirdPi), Half);
}

TEST_F(ApeironTest, Tan)
{
  EXPECT_DOUBLE_EQ(Tan(Zero), Zero);
  EXPECT_DOUBLE_EQ(Tan(SixthPi), One/Sqrt(Three));
  EXPECT_DOUBLE_EQ(Tan(QuartPi), One);
  EXPECT_NEAR(Tan(ThirdPi), Sqrt(Three), Small);
  EXPECT_THROW(Tan(HalfPi), std::domain_error);
}

TEST_F(ApeironTest, Arcsin)
{
  EXPECT_DOUBLE_EQ(Arcsin(-One), -HalfPi);
  EXPECT_DOUBLE_EQ(Arcsin(Zero), Zero);
  EXPECT_DOUBLE_EQ(Arcsin(One), HalfPi);

  EXPECT_DOUBLE_EQ(Arcsin(Half), SixthPi);
  EXPECT_DOUBLE_EQ(Arcsin(One/Sqrt(Two)), QuartPi);
  EXPECT_DOUBLE_EQ(Arcsin(Half*Sqrt(Three)), ThirdPi);
}

TEST_F(ApeironTest, Arccos)
{
  EXPECT_DOUBLE_EQ(Arccos(-One), Pi);
  EXPECT_DOUBLE_EQ(Arccos(Zero), HalfPi);
  EXPECT_DOUBLE_EQ(Arccos(One), Zero);

  EXPECT_NEAR(Arccos(Half*Sqrt(Three)), SixthPi, Small);
  EXPECT_DOUBLE_EQ(Arccos(One/Sqrt(Two)), QuartPi);
  EXPECT_DOUBLE_EQ(Arccos(Half), ThirdPi);
}

}

#endif