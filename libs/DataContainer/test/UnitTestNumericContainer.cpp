#include <gtest/gtest.h>
#include "../include/NumericContainer.h"
#include "../../DataContainer/include/Array.h"

#ifdef DEBUG_MODE

constexpr size_t ContainerSize = 50;

namespace aprn {
namespace Test {

/***************************************************************************************************************************************************************
* NumericContainer Test Classes
***************************************************************************************************************************************************************/
template<typename T, class derived>
class NumericContainer : public detail::NumericContainer<T, derived>
{
protected:
  constexpr NumericContainer() = default;

public:
  constexpr derived& Derived() noexcept { return static_cast<derived&>(*this); }
  constexpr const derived& Derived() const noexcept { return static_cast<const derived&>(*this); }
};

template<typename T, size_t N>
struct StaticNumericContainer : public StaticArray<T, N>,
                               public NumericContainer<T, StaticNumericContainer<T, N>>
{

};

template<typename T>
struct DynamicNumericContainer : public DynamicArray<T>,
                                public NumericContainer<T, DynamicNumericContainer<T>>
{
  DynamicNumericContainer(const size_t size) : DynamicArray<T>::DynamicArray(size) {}
};

/***************************************************************************************************************************************************************
* NumericContainer Test Fixture
***************************************************************************************************************************************************************/
class NumericContainerTest : public testing::Test
{
public:
  Random<int> RandomInt;
  Random<Float> RandomFloat;

  StaticNumericContainer<int, ContainerSize> IntStaticContainer;
  StaticNumericContainer <Float, ContainerSize> FloatStaticContainer;

  DynamicNumericContainer<int> IntDynamicContainer;
  DynamicNumericContainer <Float> FloatDynamicContainer;

  NumericContainerTest()
    : RandomInt(-10, 10), RandomFloat(-Ten, Ten), IntDynamicContainer(ContainerSize), FloatDynamicContainer(ContainerSize) {}

  void SetUp() override
  {
    FOR_EACH(entry, IntStaticContainer) entry = RandomInt();
    FOR_EACH(entry, IntDynamicContainer) entry = RandomInt();
    FOR_EACH(entry, FloatStaticContainer) entry = RandomFloat();
    FOR_EACH(entry, FloatDynamicContainer) entry = RandomFloat();
  }
};

}
}

#endif