#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "FileSystem.h"

#include <fstream>
#include <optional>
#include <string>
#include <string_view>

namespace aprn::flmgr {

enum class Mode
{
   Read     = std::ios::in,     // Read
   Write    = std::ios::out,    // Write
   Append   = std::ios::app,    // Write
   Truncate = std::ios::trunc,  // Write
   AtEnd    = std::ios::ate,    // Read/Write
   Binary   = std::ios::binary, // Read/Write
};

template<typename T>
concept ModeType = isTypeSame<T, Mode>();

class File
{
 public:
   File() = default;

   File(const fs::path& file_path, ModeType auto... open_modes);

   ~File();

   void Open(const fs::path& file_path, ModeType auto... open_modes);

   void Close();

   auto ReadLine(std::string& line);

   std::string ReadLine();

   void Write(const std::string_view& str);

   template<char sep = '\0', typename T, typename... Ts>
   void Write(const T& data, const Ts&... trailing_data);

   bool isReadable() const;

   bool isWritable() const;

   inline bool isOpen() const { return _Stream.is_open(); }

   inline bool isEnd() const { return _Stream.eof(); }

   inline void SetPrecision(const unsigned n) { _Stream << std::setprecision(n); }

 private:
   void Check(const fs::path& file_path) const;

   fs::path                    _Path;
   std::fstream                _Stream;
   DArray<Mode>                _Modes;
   mutable std::optional<bool> _isReadable;
   mutable std::optional<bool> _isWritable;
};

}

#include "File.tpp"
