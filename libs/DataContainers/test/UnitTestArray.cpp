#include <gtest/gtest.h>
#include "../include/Array.h"
#include "../include/MultiArray.h"

#ifdef DEBUG_MODE

constexpr std::size_t ArraySize = 10;

namespace Apeiron
{

class ArrayTest : public testing::Test
{
  public:

  StaticArray<Bool, ArraySize> BoolStaticArray;
  StaticArray<int, ArraySize> IntStaticArray;
  StaticArray<Float, ArraySize> FloatStaticArray;
  StaticArray<std::string, ArraySize> StringStaticArray;

  DynamicArray<Bool> BoolDynamicArray;
  DynamicArray<int> IntDynamicArray;
  DynamicArray<Float> FloatDynamicArray;
  DynamicArray<std::string> StringDynamicArray;

  void SetUp() override
  {
    BoolDynamicArray.resize(ArraySize);
    IntDynamicArray.resize(ArraySize);
    FloatDynamicArray.resize(ArraySize);
    StringDynamicArray.resize(ArraySize);
  }
};

TEST_F(ArrayTest, SubscriptOperator)
{
  FOR_EACH(entry, BoolStaticArray) EXPECT_EQ(entry, false);
  FOR_EACH(entry, IntStaticArray) EXPECT_EQ(entry, false);
}

}

#endif

