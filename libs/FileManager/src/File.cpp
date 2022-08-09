#include "../include/File.h"

#include <iomanip>
#include <iostream>

namespace aprn::flmgr {

File::~File() { Close(); }

void
File::Close()
{
   _Path = "";
   _Stream.close();
   _Modes.Erase();
}

auto
File::ReadLine(std::string& line)
{
   ASSERT(isReadable(), "The file must be readable to read a line.")
   std::getline(_Stream, line);
}

std::string
File::ReadLine()
{
   ASSERT(isReadable(), "The file must be readable to read a line.")

   std::string line;
   ReadLine(line);
   return line;
}

void
File::Write(const std::string_view& str)
{
   ASSERT(isWritable(), "The file must be writable to write a line.")
   _Stream << str;
}

bool
File::isReadable() const
{
   DEBUG_ASSERT(isOpen(), "The file is not yet open.")
   if(!_isReadable.has_value()) _isReadable = Mode::Read == OneOf(_Modes);
   return _isReadable.value();
}

bool
File::isWritable() const
{
   DEBUG_ASSERT(isOpen(), "The file is not yet open.")
   if(!_isWritable.has_value())
      _isWritable = std::any_of(_Modes.begin(), _Modes.end(), [](auto mode){ return mode == OneOf(Mode::Write, Mode::Append, Mode::Truncate); });
   return _isWritable.value();
}

void
File::Check(const fs::path& file_path) const
{
   ASSERT(!isDirectory(file_path), "The following is a directory, not a file: ", file_path.filename())
   ASSERT(FileExists(file_path), "The file ", file_path.filename(), " was not found.")
   ASSERT(!_Stream.fail(), "Failed to open file: ", file_path.filename())
   ASSERT(isReadable() != isWritable(), "A file must be opened in either a read or a write mode.")
}

}