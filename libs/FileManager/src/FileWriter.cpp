#include "FileManager/include/FileWriter.h"

namespace aprn::flmgr {

void
FileWriter::Open(const std::string& file_name)
{
   FileIO::Open(file_name);
}

}