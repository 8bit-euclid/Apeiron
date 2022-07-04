#pragma once

#include "FileIO.h"

namespace aprn::flmgr {

class FileWriter : public FileIO
{
 public:
   FileWriter() = default;

   FileWriter(const std::string& file_name);

   void Open(const std::string& file_name);
};

}
