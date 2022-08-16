#pragma once

namespace aprn::flmgr {

template<bool wide>
BaseFile<wide>::BaseFile(const Path& file_path, ModeType auto... open_modes)
   : _Path(file_path), _Stream(file_path,
     (static_cast<std::ios_base::openmode>(open_modes) | ...)), _Modes({open_modes...}) { Init(file_path); }

template<bool wide>
void
BaseFile<wide>::Open(const Path& file_path, ModeType auto... open_modes)
{
   ASSERT(!isOpen(), "The file ", file_path.filename(), " is already open.")

   _Path  = file_path;
   _Modes = { open_modes... };
   _Stream.open(file_path, (static_cast<std::ios_base::openmode>(open_modes) | ...));

   Init(file_path);
}

template<bool wide>
template<char sep, typename T, typename... Ts>
void
BaseFile<wide>::Read(T& data, Ts&... trailing_data)
{
   DEBUG_ASSERT(isReadable(), "The file must be readable to read data.")

   _Stream >> data;
   if constexpr(sizeof...(Ts))
   {
      if constexpr(sep != '\0')
      {
         DEBUG_ASSERT(_Stream.peek() == sep, "The data to be read in is not delimitted by a '", sep, "' character.")
         _Stream >> Unmove(char());
      }
      Read<sep>(trailing_data...);
   }
}

template<bool wide>
template<char sep, typename T, typename... Ts>
void
BaseFile<wide>::Write(const T& data, const Ts&... trailing_data)
{
   DEBUG_ASSERT(isWritable(), "The file must be writable to write a line.")

   _Stream << data;
   if constexpr(sizeof...(Ts))
   {
      if constexpr(sep != '\0') _Stream << sep;
      Write<sep>(trailing_data...);
   }
}

}