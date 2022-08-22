/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

#include "../include/FileSystem.h"

namespace aprn::flmgr {

/***************************************************************************************************************************************************************
* File Handling Functions
***************************************************************************************************************************************************************/
void
CopyFile(const Path& from_file_path, const Path& to_dir_path)
{
   ASSERT(FileExists(from_file_path), "The file ", from_file_path.filename(), " does not exist.")

   Path target = to_dir_path / from_file_path.filename();
   if(!FileExists(to_dir_path)) fs::create_directories(to_dir_path); // Recursively create target directory, if it doesn't exist.
   fs::copy_file(from_file_path, target, fs::copy_options::overwrite_existing);
}

/***************************************************************************************************************************************************************
* Directory Handling Functions
***************************************************************************************************************************************************************/
void
CopyDirectory(const Path& from_dir_path, const Path& to_dir_path)
{
   ASSERT(DirectoryExists(from_dir_path), "The directory ", from_dir_path.filename(), " does not exist.")

   Path target = to_dir_path / from_dir_path.filename();
   fs::create_directories(to_dir_path); // Recursively create target directory, if it doesn't exist.
   fs::copy(from_dir_path, target, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
}

}