#include <gtest/gtest.h>
#include "../include/Vector.h"
#include "../include/VectorOperations.h"

#ifdef DEBUG_MODE

constexpr std::size_t ContainerSize = 50;

namespace Apeiron {

class VectorTest : public testing::Test
{
public:
  StaticVector<int, ContainerSize> IntStaticVector;
  StaticVector<Float, ContainerSize> FloatStaticVector;

  DynamicVector<int> IntDynamicVector;
  DynamicVector<Float> FloatDynamicVector;

  VectorTest()
    : IntDynamicVector(ContainerSize), FloatDynamicVector(ContainerSize) {}

  void SetUp() override
  {

  }
};

}

#endif