#include "../include/File.h"

#include <iomanip>
#include <iostream>

namespace aprn::flmgr {

/***************************************************************************************************************************************************************
* File Public Interface
***************************************************************************************************************************************************************/
template<bool wide>
BaseFile<wide>::~BaseFile() { if(isOpen()) Close(); }

template<bool wide>
void
BaseFile<wide>::Close()
{
   ASSERT(isOpen(), "The file ", _Path.filename(), " had not been open yet.")

   _Path = "";
   _Stream.close();
   _Modes.Erase();
   _isReadable.reset();
   _isWritable.reset();
}

template<bool wide>
void
BaseFile<wide>::Read(ConditionalType<std::string, std::wstring>& line)
{
   DEBUG_ASSERT(isReadable(), "The file must be readable to read a line.")
   std::getline(_Stream, line);
}

template<bool wide>
void
BaseFile<wide>::Write(const ConditionalType<std::string_view, std::wstring_view>& str)
{
   DEBUG_ASSERT(isWritable(), "The file must be writable to write a line.")
   _Stream << str;
}

/***************************************************************************************************************************************************************
* File Private Interface
***************************************************************************************************************************************************************/
template<bool wide>
void
BaseFile<wide>::Init(const Path& file_path)
{
   ASSERT(!isDirectory(file_path), "The following is a directory, not a file: ", file_path.filename())
   ASSERT(FileExists(file_path), "The file ", file_path.filename(), " was not found.")
   ASSERT(!_Stream.fail(), "Failed to open file: ", file_path.filename())
   ASSERT(isReadable() != isWritable(), "A file must be opened in either a read or a write mode.")

   // Note, if wide file, assumes UTF-8 encoding. Need to modify if encoded differently.
   if constexpr(wide)
   {
      std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
      _Stream.imbue(utf8_locale);
   }
}

template<bool wide>
bool
BaseFile<wide>::isReadable() const
{
   DEBUG_ASSERT(isOpen(), "The file is not yet open.")
   if(!_isReadable.has_value()) _isReadable = Mode::Read == OneOf(_Modes);
   return _isReadable.value();
}

template<bool wide>
bool
BaseFile<wide>::isWritable() const
{
   DEBUG_ASSERT(isOpen(), "The file is not yet open.")
   if(!_isWritable.has_value())
      _isWritable = std::any_of(_Modes.begin(), _Modes.end(), [](auto m){ return m == OneOf(Mode::Write, Mode::Append, Mode::Truncate); });
   return _isWritable.value();
}

template class BaseFile<true>;
template class BaseFile<false>;
}