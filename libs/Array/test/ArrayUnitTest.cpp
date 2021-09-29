#include <gtest/gtest.h>

/** Run all unit tests in this file. */
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class ArrayTest : public testing::Test
{

};

TEST_F(ArrayTest, Test1)
{

}

