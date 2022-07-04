#pragma once

#include "FileIO.h"

namespace aprn::flmgr {

class FileReader : public FileIO
{
 public:
   FileReader() = default;

   FileReader(const std::string& file_name);

   void Open(const std::string& file_name);
};

}
