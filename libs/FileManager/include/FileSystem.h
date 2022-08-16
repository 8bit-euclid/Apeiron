#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"

#include <filesystem>
#include <stdlib.h>
#include <string_view>

namespace aprn::flmgr {

namespace fs = std::filesystem;
typedef fs::path Path;

/***************************************************************************************************************************************************************
* File Handling Functions
***************************************************************************************************************************************************************/
inline Path
FileName(const std::string_view& file_path) { return Path(file_path).filename(); }

inline bool
FileExists(const Path& file_path) { return fs::exists(file_path); }

inline bool
FileIsEmpty(const Path& file_path)
{
   ASSERT(FileExists(file_path), "The file/directory ", file_path.filename(), " does not exist.")
   return fs::is_empty(file_path);
}

inline void
ClearFile(const Path& file_path)
{
   ASSERT(FileExists(file_path), "The file ", file_path.filename(), " does not exist.")
   fs::resize_file(file_path, 0);
}

inline bool
DeleteFile(const Path& file_path)
{
   ASSERT(FileExists(file_path), "The file/directory ", file_path.filename(), " does not exist.")
   return fs::remove(file_path);
}

inline void
MoveFile(const Path& from_file_path, const Path& to_dir_path)
{
   ASSERT(FileExists(from_file_path), "The file ", from_file_path.filename(), " does not exist.")

   Path target = to_dir_path / from_file_path.filename();
   fs::rename(from_file_path, target);
}

void
CopyFile(const Path& from_file_path, const Path& to_dir_path);

/***************************************************************************************************************************************************************
* Directory Handling Functions
***************************************************************************************************************************************************************/
inline bool
isDirectory(const Path& path) { return fs::is_directory(path); }

inline bool
DirectoryExists(const Path& dir_path) { return FileExists(dir_path) && isDirectory(dir_path); }

inline void
CreateDirectory(const Path& dir_path)
{
   ASSERT(DirectoryExists(dir_path.parent_path()), "The parent directory of the given path ", dir_path.filename(), " does not exist.")
   fs::create_directory(dir_path);
}

inline void
CreateDirectories(const Path& dir_path) { fs::create_directories(dir_path); }

inline bool
DirectoryIsEmpty(const Path& dir_path)
{
   ASSERT(DirectoryExists(dir_path), "The directory ", dir_path.filename(), " does not exist.")
   return FileIsEmpty(dir_path) && isDirectory(dir_path);
}

inline void
ClearDirectory(const Path& dir_path) { FOR_EACH(entry, fs::directory_iterator(dir_path)) fs::remove_all(entry.path()); }

inline bool
DeleteDirectory(const Path& dir_path)
{
   ClearDirectory(dir_path);
   return DeleteFile(dir_path);
}

void
CopyDirectory(const Path& from_dir_path, const Path& to_dir_path);

inline void
MoveDirectory(const std::string_view& from_dir_path, const std::string_view& to_dir_path)
{
   CopyDirectory(from_dir_path, to_dir_path);
   DEBUG_WARN_IF(!DeleteDirectory(from_dir_path), "Failed to delete the directory ", Path(from_dir_path).filename(), " during move operation.")
}

/***************************************************************************************************************************************************************
* General Linux Commands
***************************************************************************************************************************************************************/
inline int
RunCommand(const std::string& cmd, const bool show_output = false)
{
   DEBUG_ASSERT(system(nullptr), "The command processor was not found.")
   return system((cmd + (!show_output ? " >> /dev/null" : "")).c_str());
}

inline int
RunCommandFrom(const std::string& cmd, const Path& dir_path, const bool show_output = false)
{
   ASSERT(isDirectory(dir_path), "The given path must be a directory.")
   return RunCommand("cd " + dir_path.string() + "; " + cmd, show_output);
}

inline void
RunCommands(const DArray<std::string>& cmds, const bool show_output = false) { FOR_EACH_CONST(cmd, cmds) RunCommand(cmd, show_output); }

inline void
RunCommandsFrom(const DArray<std::string>& cmds, const Path& dir_path, const bool show_output = false)
{
   FOR_EACH_CONST(cmd, cmds) RunCommandFrom(cmd, dir_path, show_output);
}

inline bool
CommandExists(const std::string& cmd)
{
   ASSERT(!isSubstring(" " , cmd), "Cannot have spaces in the Linux command: ", cmd)
   return RunCommand("which " + cmd) == 0;
}

inline void
CompileTeXFile(const std::string& tex_compiler, const Path& tex_path)
{
   ASSERT(CommandExists(tex_compiler), "Please install ", tex_compiler, " to compile a .tex file or use a different compiler.")
   ASSERT(tex_path.extension() == ".tex", "The given file has the extension ", tex_path.extension(), ". Expected a .tex file.")
   RunCommandFrom(tex_compiler + " " + tex_path.filename().string(), tex_path.parent_path());
}

inline void
ConvertPDFtoPNG(Path pdf_path, const unsigned pixel_density = 600)
{
   ASSERT(CommandExists("magick"), "Please install Magick to convert a .pdf file to a .png file.")
   ASSERT(pdf_path.extension() == ".pdf", "The given file has the extension ", pdf_path.extension(), ". Expected a .pdf file.")

   std::string cmd;
   cmd += "magick -density " + ToStr(pixel_density) + " ";
   cmd += pdf_path;
   cmd += " -colorspace RGB ";
   cmd += pdf_path.replace_extension(".png");

   RunCommand(cmd);
}

}
