#include "../include/FileSystem.h"

namespace aprn::flmgr {

/***************************************************************************************************************************************************************
* File Handling Functions
***************************************************************************************************************************************************************/
void
CopyFile(const fs::path& from_file_path, const fs::path& to_dir_path)
{
   ASSERT(FileExists(from_file_path), "The file ", from_file_path.filename(), " does not exist.")

   fs::path target = to_dir_path / from_file_path.filename();
   if(!FileExists(to_dir_path)) fs::create_directories(to_dir_path); // Recursively create target directory, if it doesn't exist.
   fs::copy_file(from_file_path, target, fs::copy_options::overwrite_existing);
}

/***************************************************************************************************************************************************************
* Directory Handling Functions
***************************************************************************************************************************************************************/
void
CopyDirectory(const fs::path& from_dir_path, const fs::path& to_dir_path)
{
   ASSERT(DirectoryExists(from_dir_path), "The directory ", from_dir_path.filename(), " does not exist.")

   fs::path target = to_dir_path / from_dir_path.filename();
   fs::create_directories(to_dir_path); // Recursively create target directory, if it doesn't exist.
   fs::copy(from_dir_path, target, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
}

}