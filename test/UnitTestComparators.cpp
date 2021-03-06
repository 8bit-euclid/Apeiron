#include <gtest/gtest.h>
#include "../include/Global.h"
#include "ApeironTest.h"

#ifdef DEBUG_MODE

namespace aprn{

TEST_F(ApeironTest, isEqual)
{
  const Float rela_tol = RelativeTolerance;
  const Float zero_tol = ZeroTolerance;

  // Zero toleranced equality (Floats only)
  EXPECT_TRUE(isEqual(Zero, Zero));
  EXPECT_TRUE(isEqual(Zero, 0.999 * zero_tol));
  EXPECT_TRUE(isEqual(0.999 * zero_tol, Zero));
  EXPECT_FALSE(isEqual(Zero, zero_tol));
  EXPECT_FALSE(isEqual(zero_tol, Zero));
  EXPECT_FALSE(isEqual(Zero, One));

  FOR(i, 1000)
  {
    const int random_int = RandomInt();
    const Float random_float = RandomFloat();

    // Exact precision equality
    EXPECT_TRUE(isEqual<true>(random_int, random_int));
    EXPECT_TRUE(isEqual<true>(random_float, random_float));
    EXPECT_FALSE(isEqual<true>(random_int, random_int + 1));
    EXPECT_FALSE(isEqual<true>(random_float, random_float + Small));

    // Relative toleranced equality (Floats only)
    EXPECT_TRUE(isEqual(random_float, random_float));
    EXPECT_TRUE(isEqual(random_float, random_float + 0.9 * rela_tol * random_float));
    EXPECT_TRUE(isEqual(random_float + 0.9 * rela_tol * random_float, random_float));
    EXPECT_TRUE(isEqual(random_float, random_float - 0.9 * rela_tol * random_float));
    EXPECT_TRUE(isEqual(random_float - 0.9 * rela_tol * random_float, random_float));
    EXPECT_FALSE(isEqual(random_float, random_float + 1.1 * rela_tol * random_float));
    EXPECT_FALSE(isEqual(random_float + 1.1 * rela_tol * random_float, random_float));
    EXPECT_FALSE(isEqual(random_float, random_float - 1.1 * rela_tol * random_float));
    EXPECT_FALSE(isEqual(random_float - 1.1 * rela_tol * random_float, random_float));
    EXPECT_FALSE(isEqual(random_float, random_float + One));
  }
}

TEST_F(ApeironTest, isLess)
{
  const Float rela_tol = RelativeTolerance;
  const Float zero_tol = ZeroTolerance;

  // Zero toleranced lesser than (Floats only)
  EXPECT_TRUE(isLess(Zero, One));
  EXPECT_TRUE(isLess(Zero, zero_tol));
  EXPECT_FALSE(isLess(Zero, 0.999 * zero_tol));
  EXPECT_FALSE(isLess(Zero, Zero));

  FOR(i, 1000)
  {
    const int random_int = RandomInt();
    const Float random_float = RandomFloat();

    // Exact precision lesser than
    EXPECT_TRUE(isLess<true>(random_int, random_int + 1));
    EXPECT_TRUE(isLess<true>(random_float, random_float + Small));
    EXPECT_FALSE(isLess<true>(random_int, random_int));
    EXPECT_FALSE(isLess<true>(random_float, random_float));

    // Relative toleranced lesser than (Floats only)
    EXPECT_TRUE(isLess(random_float, random_float + 1.1 * rela_tol * Abs(random_float)));
    EXPECT_TRUE(isLess(random_float - 1.1 * rela_tol * Abs(random_float), random_float));
    EXPECT_TRUE(isLess(random_float, random_float + One));
    EXPECT_FALSE(isLess(random_float, random_float));
    EXPECT_FALSE(isLess(random_float, random_float + 0.9 * rela_tol * Abs(random_float)));
    EXPECT_FALSE(isLess(random_float - 0.9 * rela_tol * Abs(random_float), random_float));
  }
}

TEST_F(ApeironTest, isLessEqual)
{
  const Float rela_tol = RelativeTolerance;
  const Float zero_tol = ZeroTolerance;

  // Zero toleranced lesser than or equal to (Floats only)
  EXPECT_TRUE(isLessEqual(Zero, Zero));
  EXPECT_TRUE(isLessEqual(Zero, One));
  EXPECT_TRUE(isLessEqual(Zero, 0.999 * zero_tol));
  EXPECT_FALSE(isLessEqual(1.111 * zero_tol, Zero));

  FOR(i, 1000)
  {
    const int random_int = RandomInt();
    const Float random_float = RandomFloat();

    // Exact precision lesser than or equal to
    EXPECT_TRUE(isLessEqual<true>(random_int, random_int));
    EXPECT_TRUE(isLessEqual<true>(random_float, random_float));
    EXPECT_TRUE(isLessEqual<true>(random_int, random_int + 1));
    EXPECT_TRUE(isLessEqual<true>(random_float, random_float + Small));

    // Relative toleranced lesser than or equal to (Floats only)
    EXPECT_TRUE(isLessEqual(random_float, random_float));
    EXPECT_TRUE(isLessEqual(random_float, random_float + One));
    EXPECT_TRUE(isLessEqual(random_float, random_float + 0.9 * rela_tol * Abs(random_float)));
    EXPECT_TRUE(isLessEqual(random_float - 1.1 * rela_tol * Abs(random_float), random_float));
    EXPECT_FALSE(isLessEqual(random_float + 1.1 * rela_tol * Abs(random_float), random_float));
  }
}

TEST_F(ApeironTest, isLarger)
{
  FOR(i, 1000)
  {
    const int random_int = RandomInt();
    const Float random_float = RandomFloat();

    EXPECT_EQ(isLarger<true>(random_int, random_int), !isLessEqual<true>(random_int, random_int));
    EXPECT_EQ(isLarger<true>(random_float, random_float), !isLessEqual<true>(random_float, random_float));
    EXPECT_EQ(isLarger(random_float, random_float), !isLessEqual(random_float, random_float));
  }
}

TEST_F(ApeironTest, isLargerEqual)
{
  FOR(i, 1000)
  {
    const int random_int = RandomInt();
    const Float random_float = RandomFloat();

    EXPECT_EQ(isLargerEqual<true>(random_int, random_int), !isLess<true>(random_int, random_int));
    EXPECT_EQ(isLargerEqual<true>(random_float, random_float), !isLess<true>(random_float, random_float));
    EXPECT_EQ(isLargerEqual(random_float, random_float), !isLess(random_float, random_float));
  }
}

TEST_F(ApeironTest, isBounded)
{
  FOR(i, 1000)
  {
    const int min_int = -Abs(RandomInt());
    const int max_int = min_int + 1 + Abs(RandomInt());
    const Float min_float = -Abs(RandomFloat());
    const Float max_float = min_float + One + Abs(RandomFloat());

    // Test left bounds
    EXPECT_TRUE(isBounded(min_int, min_int, max_int));
    EXPECT_TRUE(isBounded<true>(min_int, min_int, max_int));
    EXPECT_FALSE(isBounded<false>(min_int, min_int, max_int));
    EXPECT_TRUE(isBounded(min_float, min_float, max_float));
    EXPECT_TRUE(isBounded<true>(min_float, min_float, max_float));
    EXPECT_FALSE(isBounded<false>(min_float, min_float, max_float));

    // Test right bounds
    EXPECT_FALSE(isBounded(max_int, min_int, max_int));
    EXPECT_FALSE((isBounded<true, false>(max_int, min_int, max_int)));
    EXPECT_TRUE((isBounded<true, true>(max_int, min_int, max_int)));
    EXPECT_FALSE(isBounded(max_float, min_float, max_float));
    EXPECT_FALSE((isBounded<true, false>(max_float, min_float, max_float)));
    EXPECT_TRUE((isBounded<true, true>(max_float, min_float, max_float)));

    // Test inclusion/exclusion
//    const int random_int = RandomInt();
//    const Float random_float = RandomFloat();
//
//    EXPECT_EQ((isBounded<false, false, true>(random_int, min_int, max_int)), ());
//    EXPECT_EQ(isBounded<true>(random_int, min_int, max_int));
//    EXPECT_EQ(isBounded<false>(random_int, min_int, max_int));
//    EXPECT_EQ(isBounded(random_float, min_float, max_float));
//    EXPECT_EQ(isBounded<true>(random_float, min_float, max_float));
//    EXPECT_EQ(isBounded<false>(random_float, min_float, max_float));

  }
}

}

#endif