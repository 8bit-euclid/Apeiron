#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace aprn::flmgr {

enum class OpenMode
{
   Read     = std::ios::in,
   Write    = std::ios::out,
   Append   = std::ios::app,
   Truncate = std::ios::trunc,
   AtEnd    = std::ios::ate,
   Binary   = std::ios::binary,
};

class FileIO
{
 public:
   FileIO() = default;

   FileIO(const std::string& file_name, Enumerator auto... open_modes);

   ~FileIO();

   void Open(const std::string& file_name, Enumerator auto... open_modes);

   void Close();

   void GetChar(char& ch);

   void GetLine(std::string& line);

   inline bool isGood() const { return FileStream.good(); }

   inline bool isOpen() const { return FileStream.is_open(); }

   inline bool isEnd() const { return FileStream.eof(); }

 private:
   std::string            FileName;
   std::fstream           FileStream;
   DynamicArray<OpenMode> OpenModes;
   bool                   isFileOpen;
};

}
