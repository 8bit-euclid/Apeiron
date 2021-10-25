#include <gtest/gtest.h>
#include "../include/Global.h"
#include "ApeironTest.h"

#ifdef DEBUG_MODE

namespace Apeiron{

/***************************************************************************************************************************************************************
* Sequences and Series
***************************************************************************************************************************************************************/
/** isEqual: Test . */
TEST_F(ApeironTest, GetInteger)
{
  constexpr std::size_t size = 100;
  FOR(i, size) EXPECT_EQ(GetInteger(std::make_integer_sequence<int, size>{}, i), i);
}

/***************************************************************************************************************************************************************
* General Arithmetic Functions
***************************************************************************************************************************************************************/
/** isLess: Test . */
TEST_F(ApeironTest, Sum)
{
  constexpr int intSum = Sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  EXPECT_EQ(intSum, 55);

  constexpr Float floatSum = Sum(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0);
  EXPECT_DOUBLE_EQ(floatSum, 55.0);
}

/** isLessEqual: Test . */
TEST_F(ApeironTest, Multiply)
{
  constexpr int intProd = Multiply(1, 2, 3, 4, 5);
  EXPECT_EQ(intProd, 120);

  constexpr Float floatProd = Multiply(1.0, 2.0, 3.0, 4.0, 5.0);
  EXPECT_DOUBLE_EQ(floatProd, 120.0);
}

/** isLarger: Test . */
TEST_F(ApeironTest, Divide)
{
  EXPECT_EQ(Divide(20, 5), Four);
  EXPECT_THROW(Divide(1, 0), std::logic_error);

  EXPECT_EQ(Divide(20.0, 5.0), Four);
  EXPECT_THROW(Divide(1.0, 0.0), std::logic_error);
}

/** isLargerEqual: Test . */
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
/** isBounded: Test . */
TEST_F(ApeironTest, Factorial)
{
  EXPECT_EQ(Factorial(0), 1);
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(5), 120);
}

/** isBounded: Test . */
TEST_F(ApeironTest, FactorialQuotient)
{
  EXPECT_EQ(FactorialQuotient(1, 0), 1);
  EXPECT_EQ(FactorialQuotient(2, 1), 2);
  EXPECT_EQ(FactorialQuotient(2, 2), 1);
  EXPECT_EQ(FactorialQuotient(5, 3), 20);
  EXPECT_THROW(FactorialQuotient(2, 3), std::logic_error);
}

/** isBounded: Test . */
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
/** isBounded: Test . */
TEST_F(ApeironTest, iPow)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Square)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Cube)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Sqrt)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Cbrt)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Hypot)
{

}

/** isBounded: Test . */
TEST_F(ApeironTest, Exp)
{

}

}

#endif