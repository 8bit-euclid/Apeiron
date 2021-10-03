#include <gtest/gtest.h>
#include "../include/Global.h"

#ifdef DEBUG_MODE

namespace Apeiron
{

///** Run all unit tests in this file. */
//int main(int argc, char** argv)
//{
//  testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}

class ApeironTest : public testing::Test
{
  public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  ApeironTest() : RandomInt(-10, 10), RandomFloat(-Ten, Ten) {}

//  void SetUp() override
//  {
//
//  }
};

/** Min: Test min function. */
TEST_F(ApeironTest, Min)
{
  const int integer = RandomInt();
  EXPECT_EQ(Min(integer, integer + 1), integer);
  EXPECT_EQ(Min(integer, integer - 1), integer - 1);

  const Float floating = RandomFloat();
  EXPECT_EQ(Min(floating, floating + One), floating);
  EXPECT_EQ(Min(floating, floating - One), floating - One);
}

/** Max: Test max function. */
TEST_F(ApeironTest, Max)
{
  const int integer = RandomInt();
  EXPECT_EQ(Max(integer, integer + 1), integer + 1);
  EXPECT_EQ(Max(integer, integer - 1), integer);

  const Float floating = RandomFloat();
  EXPECT_EQ(Max(floating, floating + One), floating + One);
  EXPECT_EQ(Max(floating, floating - One), floating);
}

/** MinMax: Test minmax function. */
TEST_F(ApeironTest, MinMax)
{
  const int minInt = RandomInt();
  const int maxInt = minInt + 1;
  EXPECT_EQ(MinMax(minInt, maxInt).first, minInt);
  EXPECT_EQ(MinMax(minInt, maxInt).second, maxInt);
  EXPECT_EQ(MinMax(maxInt, minInt).first, minInt);
  EXPECT_EQ(MinMax(maxInt, minInt).second, maxInt);

  const Float minFloat = RandomFloat();
  const Float maxFloat = minFloat + One;
  EXPECT_EQ(MinMax(minFloat, maxFloat).first, minFloat);
  EXPECT_EQ(MinMax(minFloat, maxFloat).second, maxFloat);
  EXPECT_EQ(MinMax(maxFloat, minFloat).first, minFloat);
  EXPECT_EQ(MinMax(maxFloat, minFloat).second, maxFloat);
}

/** Bound: Test the bounding function. */
TEST_F(ApeironTest, Bound)
{
  const int minInt = -Abs(RandomInt());
  const int maxInt = Abs(RandomInt()) + 1;
  const int integer = RandomInt();
  EXPECT_GE(Bound(integer, minInt, maxInt), minInt);
  EXPECT_LE(Bound(integer, minInt, maxInt), maxInt);

  const Float minFloat = -Abs(RandomFloat());
  const Float maxFloat = Abs(RandomFloat()) + One;
  const Float floating = RandomFloat();
  EXPECT_GE(Bound(floating, minFloat, maxFloat), minFloat);
  EXPECT_LE(Bound(floating, minFloat, maxFloat), maxFloat);
}

/** Sgn: Test the signum function. */
TEST_F(ApeironTest, Sgn)
{
  const int negativeInt = -Abs(RandomInt()) - 1;
  const int positiveInt = Abs(RandomInt()) + 1;
  EXPECT_EQ(Sgn(negativeInt), -1);
  EXPECT_EQ(Sgn(positiveInt), 1);
  EXPECT_EQ(Sgn(0), 1);
  EXPECT_EQ(Sgn(0, 1), 1);
  EXPECT_EQ(Sgn(0, 0), 0);
  EXPECT_EQ(Sgn(0, -1), -1);

  const Float negativeFloat = -Abs(RandomFloat()) - One;
  const Float positiveFloat = Abs(RandomFloat()) + One;
  EXPECT_EQ(Sgn(negativeFloat), -One);
  EXPECT_EQ(Sgn(positiveFloat), One);
  EXPECT_EQ(Sgn(Zero), One);
  EXPECT_EQ(Sgn(Zero, 1), One);
  EXPECT_EQ(Sgn(Zero, 0), Zero);
  EXPECT_EQ(Sgn(Zero, -1), -One);
}

/** Abs: Test the absolute value function. */
TEST_F(ApeironTest, Abs)
{
  const int negativeInt = -Abs(RandomInt()) - 1;
  const int positiveInt = Abs(RandomInt()) + 1;
  EXPECT_EQ(Abs(negativeInt), -negativeInt);
  EXPECT_EQ(Abs(positiveInt), positiveInt);

  const Float negativeFloat = -Abs(RandomFloat()) - One;
  const Float positiveFloat = Abs(RandomFloat()) + One;
  EXPECT_EQ(Abs(negativeFloat), -negativeFloat);
  EXPECT_EQ(Abs(positiveFloat), positiveFloat);
}

/** Floor: Test the floor function. */
TEST_F(ApeironTest, Floor)
{
  EXPECT_EQ(Floor(-1.234), -Two);
  EXPECT_EQ(Floor(-1.678), -Two);
  EXPECT_EQ(Floor(-Two), -Two);

  EXPECT_EQ(Floor(1.234), One);
  EXPECT_EQ(Floor(1.999), One);
  EXPECT_EQ(Floor(One), One);
}

/** Ceil: Test the ceil function. */
TEST_F(ApeironTest, Ceil)
{
  EXPECT_EQ(Ceil(-1.234), -One);
  EXPECT_EQ(Ceil(-1.999), -One);
  EXPECT_EQ(Ceil(-Two), -Two);

  EXPECT_EQ(Ceil(1.234), Two);
  EXPECT_EQ(Ceil(1.999), Two);
  EXPECT_EQ(Ceil(One), One);
}

/** Round: Test the round function. */
TEST_F(ApeironTest, Round)
{
  EXPECT_EQ(Round(-1.234), -One);
  EXPECT_EQ(Round(-1.5), -One);
  EXPECT_EQ(Round(-1.999), -Two);
  EXPECT_EQ(Round(-Two), -Two);

  EXPECT_EQ(Round(1.234), One);
  EXPECT_EQ(Round(1.5), Two);
  EXPECT_EQ(Round(1.999), Two);
  EXPECT_EQ(Round(One), One);
}

}

#endif