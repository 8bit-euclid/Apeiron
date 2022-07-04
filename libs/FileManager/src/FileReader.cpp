#include "FileManager/include/FileReader.h"

namespace aprn::flmgr {

void
FileReader::Open(const std::string& file_name)
{
   FileIO::Open(file_name);
}

}