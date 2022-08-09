#include <gtest/gtest.h>
#include "../include/FileSystem.h"

#include <filesystem>
#include <string_view>

#ifdef DEBUG_MODE

namespace aprn::flmgr {

namespace fs = std::filesystem;

class FileHandlerTest : public testing::Test
{
 public:
   std::string DataDir{"/home/niran90/Dropbox/Projects/Apeiron/libs/FileManager/test/data"};

   FileHandlerTest() {}

   void
   SetUp() override {}
};

/***************************************************************************************************************************************************************
* Test File Handling Functions
***************************************************************************************************************************************************************/
TEST_F(FileHandlerTest, FileExists)
{
   EXPECT_TRUE(FileExists(DataDir + "/empty_file.txt"));
   EXPECT_FALSE(FileExists(DataDir + "/image.png"));
}

TEST_F(FileHandlerTest, FileIsEmpty)
{
   EXPECT_TRUE(FileIsEmpty(DataDir + "/empty_file.txt"));
   EXPECT_FALSE(FileIsEmpty(DataDir + "/non_empty_directory/non_empty_file.txt"));
   EXPECT_DEATH(FileIsEmpty(DataDir + "/image.png"), "");
}

TEST_F(FileHandlerTest, CopyFile)
{

}

TEST_F(FileHandlerTest, MoveFile)
{

}

/***************************************************************************************************************************************************************
* Test Directory Handling Functions
***************************************************************************************************************************************************************/
TEST_F(FileHandlerTest, isDirectory)
{
   EXPECT_TRUE(isDirectory(DataDir + "/empty_directory")); // Existing directory
   EXPECT_FALSE(isDirectory(DataDir + "/empty_file.txt")); // Existing file
   EXPECT_FALSE(isDirectory(DataDir + "/images"));         // Non-existant file/directory
}

TEST_F(FileHandlerTest, DirectoryExists)
{
   EXPECT_TRUE(DirectoryExists(DataDir + "/empty_directory"));
   EXPECT_FALSE(DirectoryExists(DataDir + "/data"));
}

TEST_F(FileHandlerTest, DirectoryIsEmpty)
{
   EXPECT_TRUE(DirectoryIsEmpty(DataDir + "/empty_directory"));
   EXPECT_FALSE(DirectoryIsEmpty(DataDir + "/non_empty_directory"));
   EXPECT_DEATH(DirectoryIsEmpty(DataDir + "/non_existant_directory"), "");
}

TEST_F(FileHandlerTest, CopyDirectory)
{

}

TEST_F(FileHandlerTest, MoveDirectory)
{

}

}

#endif