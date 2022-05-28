#include <gtest/gtest.h>

#include "../../../include/Global.h"
#include "../include/Vector.h"
#include "../include/VectorOperations.h"

#ifdef DEBUG_MODE

constexpr size_t ContainerSize = 50;

namespace aprn {

/***************************************************************************************************************************************************************
* Vector Test Fixture
***************************************************************************************************************************************************************/
class VectorTest : public testing::Test
{
public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  StaticVector<int, ContainerSize> IntStaticVector;
  StaticVector<Float, ContainerSize> FloatStaticVector;
  DynamicVector<int> IntDynamicVector;
  DynamicVector<Float> FloatDynamicVector;

  StaticVector<int, ContainerSize> IntStaticVectorTest;
  StaticVector<Float, ContainerSize> FloatStaticVectorTest;
  DynamicVector<int> IntDynamicVectorTest;
  DynamicVector<Float> FloatDynamicVectorTest;

  VectorTest()
    : RandomInt(-10, 10), RandomFloat(-Ten, Ten), IntDynamicVector(ContainerSize), FloatDynamicVector(ContainerSize),
                          IntDynamicVectorTest(ContainerSize), FloatDynamicVectorTest(ContainerSize) {}

  void SetUp() override
  {
    IntStaticVector.ResetRandomiser(-10, 10);
    IntDynamicVector.ResetRandomiser(-10, 10);
    FloatStaticVector.ResetRandomiser(-Ten, Ten);
    FloatDynamicVector.ResetRandomiser(-Ten, Ten);

    IntStaticVector.Randomise();
    IntDynamicVector.Randomise();
    FloatStaticVector.Randomise();
    FloatDynamicVector.Randomise();

    IntStaticVectorTest = IntStaticVector;
    IntDynamicVectorTest = IntDynamicVector;
    FloatStaticVectorTest = FloatStaticVector;
    FloatDynamicVectorTest = FloatDynamicVector;
  }
};

/***************************************************************************************************************************************************************
* Test Arithmetic Operations
***************************************************************************************************************************************************************/
TEST_F(VectorTest, ScalarPlus)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  IntStaticVectorTest = IntStaticVector + random_int;
  IntDynamicVectorTest = IntDynamicVector + random_int;
  FloatStaticVectorTest = FloatStaticVector + random_float;
  FloatDynamicVectorTest = FloatDynamicVector + random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] + random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] + random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] + random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] + random_float);
  }
}

TEST_F(VectorTest, ScalarMinus)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  IntStaticVectorTest = IntStaticVector - random_int;
  IntDynamicVectorTest = IntDynamicVector - random_int;
  FloatStaticVectorTest = FloatStaticVector - random_float;
  FloatDynamicVectorTest = FloatDynamicVector - random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] - random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] - random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] - random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] - random_float);
  }
}

TEST_F(VectorTest, ScalarMultiply)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  // Test pre-multiplication
  IntStaticVectorTest = random_int * IntStaticVector;
  IntDynamicVectorTest = random_int * IntDynamicVector;
  FloatStaticVectorTest = random_float * FloatStaticVector;
  FloatDynamicVectorTest = random_float * FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], random_int * IntStaticVector[i]);
    EXPECT_EQ(IntDynamicVectorTest[i], random_int * IntDynamicVector[i]);
    EXPECT_EQ(FloatStaticVectorTest[i], random_float * FloatStaticVector[i]);
    EXPECT_EQ(FloatDynamicVectorTest[i], random_float * FloatDynamicVector[i]);
  }

  // Test post-multiplication
  IntStaticVectorTest = IntStaticVector * random_int;
  IntDynamicVectorTest = IntDynamicVector * random_int;
  FloatStaticVectorTest = FloatStaticVector * random_float;
  FloatDynamicVectorTest = FloatDynamicVector * random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], random_int * IntStaticVector[i]);
    EXPECT_EQ(IntDynamicVectorTest[i], random_int * IntDynamicVector[i]);
    EXPECT_EQ(FloatStaticVectorTest[i], random_float * FloatStaticVector[i]);
    EXPECT_EQ(FloatDynamicVectorTest[i], random_float * FloatDynamicVector[i]);
  }
}

TEST_F(VectorTest, ScalarDivide)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  if(random_int == 0) random_int++;
  if(isEqual(random_float, Zero)) random_float++;

  IntStaticVectorTest = IntStaticVector / random_int;
  IntDynamicVectorTest = IntDynamicVector / random_int;
  FloatStaticVectorTest = FloatStaticVector / random_float;
  FloatDynamicVectorTest = FloatDynamicVector / random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] / random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] / random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] / random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] / random_float);
  }
}

TEST_F(VectorTest, ScalarPlusEqual)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  IntStaticVectorTest += random_int;
  IntDynamicVectorTest += random_int;
  FloatStaticVectorTest += random_float;
  FloatDynamicVectorTest += random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] + random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] + random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] + random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] + random_float);
  }
}

TEST_F(VectorTest, ScalarMinusEqual)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  IntStaticVectorTest -= random_int;
  IntDynamicVectorTest -= random_int;
  FloatStaticVectorTest -= random_float;
  FloatDynamicVectorTest -= random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] - random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] - random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] - random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] - random_float);
  }
}

TEST_F(VectorTest, ScalarMultiplyEqual)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  IntStaticVectorTest *= random_int;
  IntDynamicVectorTest *= random_int;
  FloatStaticVectorTest *= random_float;
  FloatDynamicVectorTest *= random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] * random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] * random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] * random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] * random_float);
  }
}

TEST_F(VectorTest, ScalarDivideEqual)
{
  int random_int = RandomInt();
  Float random_float = RandomFloat();

  if(random_int == 0) random_int++;
  if(isEqual(random_float, Zero)) random_float++;

  IntStaticVectorTest /= random_int;
  IntDynamicVectorTest /= random_int;
  FloatStaticVectorTest /= random_float;
  FloatDynamicVectorTest /= random_float;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], IntStaticVector[i] / random_int);
    EXPECT_EQ(IntDynamicVectorTest[i], IntDynamicVector[i] / random_int);
    EXPECT_EQ(FloatStaticVectorTest[i], FloatStaticVector[i] / random_float);
    EXPECT_EQ(FloatDynamicVectorTest[i], FloatDynamicVector[i] / random_float);
  }
}

TEST_F(VectorTest, VectorPlus)
{
  IntStaticVectorTest = IntStaticVector + IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector + IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector + FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector + FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 2 * IntStaticVector[i]);
    EXPECT_EQ(IntDynamicVectorTest[i], 2 * IntDynamicVector[i]);
    EXPECT_EQ(FloatStaticVectorTest[i], Two * FloatStaticVector[i]);
    EXPECT_EQ(FloatDynamicVectorTest[i], Two * FloatDynamicVector[i]);
  }
}

TEST_F(VectorTest, VectorMinus)
{
  IntStaticVectorTest = IntStaticVector - IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector - IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector - FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector - FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 0);
    EXPECT_EQ(IntDynamicVectorTest[i], 0);
    EXPECT_EQ(FloatStaticVectorTest[i], Zero);
    EXPECT_EQ(FloatDynamicVectorTest[i], Zero);
  }
}

TEST_F(VectorTest, VectorMultiply)
{
  IntStaticVectorTest = IntStaticVector * IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector * IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector * FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector * FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], iPow(IntStaticVector[i], 2));
    EXPECT_EQ(IntDynamicVectorTest[i], iPow(IntDynamicVector[i], 2));
    EXPECT_EQ(FloatStaticVectorTest[i], iPow(FloatStaticVector[i], 2));
    EXPECT_EQ(FloatDynamicVectorTest[i], iPow(FloatDynamicVector[i], 2));
  }
}

TEST_F(VectorTest, VectorDivide)
{
  FOR_EACH(entry, IntStaticVector) if(entry == 0) entry++;
  FOR_EACH(entry, IntDynamicVector) if(entry == 0) entry++;
  FOR_EACH(entry, FloatStaticVector) if(isEqual(entry, Zero)) entry++;
  FOR_EACH(entry, FloatDynamicVector) if(isEqual(entry, Zero)) entry++;

  IntStaticVectorTest = IntStaticVector / IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector / IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector / FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector / FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 1);
    EXPECT_EQ(IntDynamicVectorTest[i], 1);
    EXPECT_EQ(FloatStaticVectorTest[i], One);
    EXPECT_EQ(FloatDynamicVectorTest[i], One);
  }
}

TEST_F(VectorTest, VectorPlusEqual)
{
  IntStaticVectorTest = IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector;

  IntStaticVectorTest += IntStaticVector;
  IntDynamicVectorTest += IntDynamicVector;
  FloatStaticVectorTest += FloatStaticVector;
  FloatDynamicVectorTest += FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 2 * IntStaticVector[i]);
    EXPECT_EQ(IntDynamicVectorTest[i], 2 * IntDynamicVector[i]);
    EXPECT_EQ(FloatStaticVectorTest[i], Two * FloatStaticVector[i]);
    EXPECT_EQ(FloatDynamicVectorTest[i], Two * FloatDynamicVector[i]);
  }
}

TEST_F(VectorTest, VectorMinusEqual)
{
  IntStaticVectorTest = IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector;

  IntStaticVectorTest -= IntStaticVector;
  IntDynamicVectorTest -= IntDynamicVector;
  FloatStaticVectorTest -= FloatStaticVector;
  FloatDynamicVectorTest -= FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 0);
    EXPECT_EQ(IntDynamicVectorTest[i], 0);
    EXPECT_EQ(FloatStaticVectorTest[i], Zero);
    EXPECT_EQ(FloatDynamicVectorTest[i], Zero);
  }
}

TEST_F(VectorTest, VectorMultiplyEqual)
{
  IntStaticVectorTest = IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector;

  IntStaticVectorTest *= IntStaticVector;
  IntDynamicVectorTest *= IntDynamicVector;
  FloatStaticVectorTest *= FloatStaticVector;
  FloatDynamicVectorTest *= FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], iPow(IntStaticVector[i], 2));
    EXPECT_EQ(IntDynamicVectorTest[i], iPow(IntDynamicVector[i], 2));
    EXPECT_EQ(FloatStaticVectorTest[i], iPow(FloatStaticVector[i], 2));
    EXPECT_EQ(FloatDynamicVectorTest[i], iPow(FloatDynamicVector[i], 2));
  }
}

TEST_F(VectorTest, VectorDivideEqual)
{
  FOR_EACH(entry, IntStaticVector) if(entry == 0) entry++;
  FOR_EACH(entry, IntDynamicVector) if(entry == 0) entry++;
  FOR_EACH(entry, FloatStaticVector) if(isEqual(entry, Zero)) entry++;
  FOR_EACH(entry, FloatDynamicVector) if(isEqual(entry, Zero)) entry++;

  IntStaticVectorTest = IntStaticVector;
  IntDynamicVectorTest = IntDynamicVector;
  FloatStaticVectorTest = FloatStaticVector;
  FloatDynamicVectorTest = FloatDynamicVector;

  IntStaticVectorTest /= IntStaticVector;
  IntDynamicVectorTest /= IntDynamicVector;
  FloatStaticVectorTest /= FloatStaticVector;
  FloatDynamicVectorTest /= FloatDynamicVector;

  FOR(i, ContainerSize)
  {
    EXPECT_EQ(IntStaticVectorTest[i], 1);
    EXPECT_EQ(IntDynamicVectorTest[i], 1);
    EXPECT_EQ(FloatStaticVectorTest[i], One);
    EXPECT_EQ(FloatDynamicVectorTest[i], One);
  }
}

/***************************************************************************************************************************************************************
* Test Other Vector Operations
***************************************************************************************************************************************************************/
TEST_F(VectorTest, InnerProduct)
{
  // Parallel test (same/opposite directions)
  FloatStaticVector = Normalise(FloatStaticVector);
  EXPECT_NEAR(InnerProduct(FloatStaticVector, FloatStaticVector), One, Small);
  EXPECT_NEAR(InnerProduct(FloatStaticVector, -FloatStaticVector), -One, Small);

  // Orthogonal test
  EXPECT_DOUBLE_EQ(InnerProduct(xAxis2, yAxis2), Zero);
  EXPECT_DOUBLE_EQ(InnerProduct(xAxis3, yAxis3), Zero);
  EXPECT_DOUBLE_EQ(InnerProduct(xAxis3, zAxis3), Zero);
  EXPECT_DOUBLE_EQ(InnerProduct(yAxis3, zAxis3), Zero);

  // Arbitrary test
  SVectorF3 random0, random1;
  random0.Randomise();
  random1.Randomise();
  EXPECT_NEAR(InnerProduct(random0, random1), L2Norm(random0) * L2Norm(random1) * Cos(ComputeAngle(random0, random1)), Two*Small);
}

TEST_F(VectorTest, CrossProduct)
{
  SVectorF3 random0;
  random0.Randomise();
  SVectorF3 cross_product;

  // Parallel test (same/opposite directions)
  cross_product = CrossProduct(ConvertVector<2>(random0), ConvertVector<2>(random0));
  FOR_EACH(entry, cross_product) EXPECT_DOUBLE_EQ(entry, Zero);
  cross_product = CrossProduct(ConvertVector<2>(random0), ConvertVector<2>(-random0));
  FOR_EACH(entry, cross_product) EXPECT_DOUBLE_EQ(entry, Zero);
  cross_product = CrossProduct(random0, random0);
  FOR_EACH(entry, cross_product) EXPECT_DOUBLE_EQ(entry, Zero);
  cross_product = CrossProduct(random0, -random0);
  FOR_EACH(entry, cross_product) EXPECT_DOUBLE_EQ(entry, Zero);

  // Orthogonal test
  cross_product = CrossProduct(xAxis2, yAxis2);
  EXPECT_TRUE(cross_product == zAxis3);
  cross_product = CrossProduct(xAxis3, yAxis3);
  EXPECT_TRUE(cross_product == zAxis3);
  cross_product = CrossProduct(yAxis3, xAxis3);
  EXPECT_TRUE(cross_product == -zAxis3);
  cross_product = CrossProduct(yAxis3, zAxis3);
  EXPECT_TRUE(cross_product == xAxis3);
  cross_product = CrossProduct(zAxis3, xAxis3);
  EXPECT_TRUE(cross_product == yAxis3);

  // Arbitrary test
  SVectorF3 random1;
  random1.Randomise();
  EXPECT_NEAR(Magnitude(CrossProduct(random0, random1)), Magnitude(random0)*Magnitude(random1)*Sin(ComputeAngle(random0, random1)), Two*Small);
}

TEST_F(VectorTest, LpNorm)
{
  EXPECT_TRUE(false);
}

TEST_F(VectorTest, L1Norm)
{
  EXPECT_DOUBLE_EQ(L1Norm(FloatStaticVector), std::accumulate(FloatStaticVector.begin(), FloatStaticVector.end(), Zero));
  EXPECT_EQ(L1Norm(IntStaticVector), std::accumulate(IntStaticVector.begin(), IntStaticVector.end(), 0));
}

TEST_F(VectorTest, L2Norm)
{
  Float inner_prod{};
  FOR_EACH(entry, FloatStaticVector) inner_prod += iPow(entry, 2);
  EXPECT_DOUBLE_EQ(L2Norm(FloatStaticVector), Sqrt(inner_prod));

  inner_prod = Zero;
  FOR_EACH(entry, IntStaticVector) inner_prod += iPow(entry, 2);
  EXPECT_DOUBLE_EQ(L2Norm(IntStaticVector), Sqrt(inner_prod));
}

TEST_F(VectorTest, LInfNorm)
{
  EXPECT_EQ(LInfNorm(FloatStaticVector), *std::max_element(FloatStaticVector.begin(), FloatStaticVector.end()));
  EXPECT_EQ(LInfNorm(IntStaticVector), *std::max_element(IntStaticVector.begin(), IntStaticVector.end()));
}

TEST_F(VectorTest, isNormalised)
{
  EXPECT_FALSE(isNormalised(FloatStaticVector));
  FloatStaticVector = Normalise(FloatStaticVector);
  EXPECT_TRUE(isNormalised(FloatStaticVector));
}

TEST_F(VectorTest, Normalise)
{
  FloatStaticVector = Normalise(FloatStaticVector);
  Float inner_prod{};
  FOR_EACH(entry, FloatStaticVector) inner_prod += iPow(entry, 2);
  EXPECT_DOUBLE_EQ(Sqrt(inner_prod), One);
}

TEST_F(VectorTest, ComputeAngle)
{
  // Non-oriented tests
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis2, xAxis2), Zero);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis2, -xAxis2), Pi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis2, SVectorF2{One, One}), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(SVectorF2{One, One}, xAxis2), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis2, SVectorF2{-One, One}), ThreeQuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(SVectorF2{-One, One}, xAxis2), ThreeQuartPi);

  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, xAxis3), Zero);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, -xAxis3), Pi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, SVectorF3{One, One, Zero}), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, SVectorF3{-One, One, Zero}), ThreeQuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, yAxis3), HalfPi);
  EXPECT_DOUBLE_EQ(ComputeAngle(xAxis3, -yAxis3), HalfPi);

  // Oriented tests
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, xAxis2), Zero);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, -xAxis2), Pi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, SVectorF2{One, One}), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, SVectorF2{-One, One}), ThreeQuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, SVectorF2{-One, One}, -zAxis3), -ThreeQuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis2, SVectorF2{-One, -One}), -ThreeQuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(SVectorF2{One, One}, xAxis2), -QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(SVectorF2{One, One}, xAxis2, -zAxis3), QuartPi);

  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis3, xAxis3), Zero);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis3, -xAxis3), Pi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis3, SVectorF3{One, One, Zero}), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(SVectorF3{One, One, Zero}, xAxis3), -QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(SVectorF3{One, One, Zero}, xAxis3, -zAxis3), QuartPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis3, yAxis3), HalfPi);
  EXPECT_DOUBLE_EQ(ComputeAngle<true>(xAxis3, -yAxis3), -HalfPi);
}

TEST_F(VectorTest, isAligned)
{
  EXPECT_TRUE(isAligned(xAxis2, xAxis2));
  EXPECT_TRUE(isAligned(xAxis2, -xAxis2));
  EXPECT_FALSE(isAligned(xAxis2, yAxis2));
  EXPECT_FALSE(isAligned(xAxis2, -yAxis2));
  EXPECT_FALSE(isAligned(xAxis2, SVectorF2{One, One}));
  EXPECT_TRUE(isAligned(xAxis2, SVectorF2{One, One}, DegToRad(45.00001)));
  EXPECT_TRUE(isAligned(xAxis2, SVectorF2{Ten, One}));

  EXPECT_TRUE(isAligned(xAxis3, xAxis3));
  EXPECT_TRUE(isAligned(xAxis3, -xAxis3));
  EXPECT_FALSE(isAligned(xAxis3, yAxis3));
  EXPECT_FALSE(isAligned(xAxis3, -yAxis3));
  EXPECT_FALSE(isAligned(xAxis3, SVectorF3{One, One, Zero}));
  EXPECT_TRUE(isAligned(xAxis3, SVectorF3{One, One, Zero}, DegToRad(45.00001)));
  EXPECT_TRUE(isAligned(xAxis3, SVectorF3{Ten, One, Zero}));

  EXPECT_THROW(isAligned(xAxis3, SVectorF3{One, Zero, Zero}, DegToRad(90.00001)), std::domain_error);
}

}

#endif