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

template<bool wide>
class BaseFile
{
   template<typename T1, typename T2> using ConditionalType = std::conditional_t<!wide, T1, T2>;

 public:
   BaseFile() = default;

   BaseFile(const Path& file_path, ModeType auto... open_modes);

   ~BaseFile();

   void Open(const Path& file_path, ModeType auto... open_modes);

   void Close();

   void Read(ConditionalType<std::string, std::wstring>& line);

   void Write(const ConditionalType<std::string_view, std::wstring_view>& str);

   template<char sep = '\0', typename T, typename... Ts>
   void Read(T& data, Ts&... trailing_data);

   template<char sep = '\0', typename T, typename... Ts>
   void Write(const T& data, const Ts&... trailing_data);

   inline bool isOpen() const { return _Stream.is_open(); }

   inline bool isValid() const { return _Stream.good(); }

   inline bool isEnd() const { return _Stream.eof(); }

   inline void SetPrecision(const unsigned n) { _Stream << std::setprecision(n); }

 private:
   void Init(const Path& file_path);

   bool isReadable() const;

   bool isWritable() const;

   using FileStream = std::basic_fstream<ConditionalType<char, wchar_t>>;
   Path                        _Path;
   FileStream                  _Stream;
   DArray<Mode>                _Modes;
   mutable std::optional<bool> _isReadable;
   mutable std::optional<bool> _isWritable;
};

typedef BaseFile<false> File;
typedef BaseFile<true>  WFile;

}

#include "File.tpp"
