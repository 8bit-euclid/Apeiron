#include <gtest/gtest.h>
#include "../include/Global.h"

#ifdef DEBUG_MODE

namespace Apeiron
{

/** Run all unit tests in this file. */
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class ApeironTest : public testing::Test
{
  Float a;
//  std::random_device Device;
//  std::mt19937 Generator;
//  std::uniform_real_distribution<Float> FloatDist(1,6);
//
//  ApeironTest() : Generator(Device()) {}

  void SetUp() override
  {

  }


};

//TEST_F(ApeironTest, Min)
//{
//
//  EXPECT_EQ();
//}

}

#endif