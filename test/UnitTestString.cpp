#include <gtest/gtest.h>
#include "../include/Global.h"
#include "ApeironTest.h"

#ifdef DEBUG_MODE

namespace aprn {

TEST_F(ApeironTest, ToString)
{
   std::string str = ToString(1);
   EXPECT_EQ(str, "1");

   str = ToString(5) + "s";
   EXPECT_EQ(str, "5s");

   str = ToString(10.5) + "m";
   EXPECT_EQ(str, "10.5m");
}

TEST_F(ApeironTest, ToNumber)
{
   EXPECT_EQ(ToNumber("1"),            1);
   EXPECT_EQ(ToNumber<int>("1"),       1);
   EXPECT_EQ(ToNumber<long>("1"),      1);
   EXPECT_EQ(ToNumber<long long>("1"), 1);

   EXPECT_EQ(ToNumber<float>("5.5"),       5.5);
   EXPECT_EQ(ToNumber<double>("5.5"),      5.5);
   EXPECT_EQ(ToNumber<long double>("5.5"), 5.5);
   EXPECT_EQ(ToNumber<double>("5.5e10"),   5.5e10);
   EXPECT_EQ(ToNumber<double>("5.5e-3"),   0.0055);
}

TEST_F(ApeironTest, isSubstring)
{
   EXPECT_TRUE(isSubstring("T"    , "True"));
   EXPECT_TRUE(isSubstring("r"    , "True"));
   EXPECT_TRUE(isSubstring("u"    , "True"));
   EXPECT_TRUE(isSubstring("e"    , "True"));
   EXPECT_TRUE(isSubstring("rue"  , "True"));
   EXPECT_TRUE(isSubstring(""     , "True"));
   EXPECT_TRUE(isSubstring("wo wo", "Two words"));

   EXPECT_FALSE(isSubstring("T"   , "False"));
   EXPECT_FALSE(isSubstring("r"   , "False"));
   EXPECT_FALSE(isSubstring("u"   , "False"));
   EXPECT_FALSE(isSubstring("m"   , "False"));
   EXPECT_FALSE(isSubstring("p"   , "False"));
   EXPECT_FALSE(isSubstring("Pal" , "False"));
   EXPECT_FALSE(isSubstring("wo o", "Two words"));
}

TEST_F(ApeironTest, Replace)
{
   EXPECT_EQ(Replace("T"    , "D" , "True")     , "Drue");
   EXPECT_EQ(Replace("r"    , "s" , "True")     , "Tsue");
   EXPECT_EQ(Replace("u"    , "e" , "True")     , "Tree");
   EXPECT_EQ(Replace("e"    , "D" , "True")     , "TruD");
   EXPECT_EQ(Replace("rue"  , "oe", "True")     , "Toe");
   EXPECT_EQ(Replace(""     , "-" , "True")     , "-T-r-u-e-");
   EXPECT_EQ(Replace("wo wo", "u" , "Two words"), "Turds");
}

TEST_F(ApeironTest, Remove)
{
   EXPECT_EQ(Remove("T"    , "True")     , "rue");
   EXPECT_EQ(Remove("r"    , "True")     , "Tue");
   EXPECT_EQ(Remove("u"    , "True")     , "Tre");
   EXPECT_EQ(Remove("e"    , "True")     , "Tru");
   EXPECT_EQ(Remove("rue"  , "True")     , "T");
   EXPECT_EQ(Remove(""     , "True")     , "True");
   EXPECT_EQ(Remove("wo wo", "Two words"), "Trds");
}

TEST_F(ApeironTest, Split)
{
   std::vector<std::string> sub_strings;
   sub_strings.push_back("This");
   sub_strings.push_back("is");
   sub_strings.push_back("the");
   sub_strings.push_back("string");
   sub_strings.push_back("that");
   sub_strings.push_back("I");
   sub_strings.push_back("would");
   sub_strings.push_back("like");
   sub_strings.push_back("to");
   sub_strings.push_back("test.");

   std::vector<std::string> delimiters;
   delimiters.push_back(",");
   delimiters.push_back("-");
   delimiters.push_back("_");
   delimiters.push_back("(");
   delimiters.push_back("{");
   delimiters.push_back("[");

   const auto n_words = sub_strings.size();
   FOR_EACH(delimiter, delimiters)
   {
      std::string test_string = sub_strings.front();
      FOR(i, 1, n_words) test_string = test_string + delimiter + sub_strings[i];

      std::vector<std::string> test_sub_strings = Split(test_string, delimiter);
      EXPECT_EQ(test_sub_strings.size(), n_words);
      FOR(i, n_words) EXPECT_EQ(test_sub_strings[i], sub_strings[i]);
   }

   EXPECT_EQ(Split("Test character split.").size(), 21);
}

}

#endif