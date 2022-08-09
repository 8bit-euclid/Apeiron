#pragma once

namespace aprn::flmgr {

File::File(const fs::path& file_path, ModeType auto... open_modes)
   : _Path(file_path), _Stream(file_path,
     (static_cast<std::ios_base::openmode>(open_modes) | ...)), _Modes({open_modes...}) { Check(file_path); }

void
File::Open(const fs::path& file_path, ModeType auto... open_modes)
{
   ASSERT(!isOpen(), "The file ", file_path.filename(), " is already open.")

   _Path  = file_path;
   _Modes = { open_modes... };
   _Stream.open(file_path, (static_cast<std::ios_base::openmode>(open_modes) | ...));

   Check(file_path);
}

template<char sep, typename T, typename... Ts>
void
File::Write(const T& data, const Ts&... trailing_data)
{
   ASSERT(isWritable(), "The file must be writable to write a line.")

   _Stream << data;
   if constexpr(sizeof...(Ts))
   {
      if constexpr(sep != '\0') _Stream << sep;
      Write<sep>(trailing_data...);
   }
}

}