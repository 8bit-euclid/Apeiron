#include <gtest/gtest.h>
#include "../include/Array.h"
#include "../include/MultiArray.h"

#ifdef DEBUG_MODE

constexpr std::size_t ArraySize = 20;

namespace Apeiron
{

class ArrayTest : public testing::Test
{
public:

  StaticArray<bool, ArraySize> BoolStaticArray;
  StaticArray<int, ArraySize> IntStaticArray;
  StaticArray<Float, ArraySize> FloatStaticArray;

  DynamicArray<Bool> BoolDynamicArray;
  DynamicArray<int> IntDynamicArray;
  DynamicArray<Float> FloatDynamicArray;

  ArrayTest() : BoolDynamicArray(ArraySize), IntDynamicArray(ArraySize), FloatDynamicArray(ArraySize) {}

  void SetUp() override
  {

  }
};

TEST_F(ArrayTest, Initialisation)
{
  // Create constexpr StaticArray objects.
  constexpr StaticArray<bool, ArraySize> ConstexprBoolArray;
  constexpr StaticArray<int, ArraySize> ConstexprIntArray(2);
  constexpr StaticArray<Float, 5> ConstexprFloatArray1{One, Two, Three, Four, Five};
  constexpr StaticArray<Float, 3> ConstexprFloatArray2(ConstexprFloatArray1.begin(), ConstexprFloatArray1.begin() + 3);

  FOR(i, 5)
  {
    EXPECT_EQ(ConstexprFloatArray1[i], Float(i + 1));
    if(i < 3) EXPECT_EQ(ConstexprFloatArray2[i], Float(i + 1));
  }

  // Test run-time initialisation of arrays.
  FOR(i, ArraySize)
  {
    // Static arrays
    EXPECT_EQ(ConstexprBoolArray[i], false);
    EXPECT_EQ(BoolStaticArray[i], false);
    EXPECT_EQ(ConstexprIntArray[i], 2);
    EXPECT_EQ(IntStaticArray[i], -1);
    EXPECT_EQ(FloatStaticArray[i], Zero);

    // Dynamic arrays
    EXPECT_EQ(BoolDynamicArray[i], false);
    EXPECT_EQ(IntDynamicArray[i], -1);
    EXPECT_EQ(FloatDynamicArray[i], Zero);
  }
}

TEST_F(ArrayTest, AssignmentOperator)
{
  FOR(i, ArraySize)
  {
    // Static arrays
    BoolStaticArray[i] = true;
    IntStaticArray[i] = 1;
    FloatStaticArray[i] = Zero;
    EXPECT_EQ(BoolStaticArray[i], true);
    EXPECT_EQ(IntStaticArray[i], 1);
    EXPECT_EQ(FloatStaticArray[i], Zero);

    // Dynamic arrays
    BoolDynamicArray[i] = true;
    IntDynamicArray[i] = 1;
    FloatDynamicArray[i] = Zero;
    EXPECT_EQ(BoolDynamicArray[i], true);
    EXPECT_EQ(IntDynamicArray[i], 1);
    EXPECT_EQ(FloatDynamicArray[i], Zero);
  }
}

}

#endif

