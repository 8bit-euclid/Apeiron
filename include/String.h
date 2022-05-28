#pragma once

#include <regex>
#include <string>
#include <vector>

namespace aprn {

/** Convert to a string. */
template<typename T>
inline std::string
ToString(const T& _number)
{
   std::stringstream str_buffer;
   str_buffer << _number << std::endl;
   std::string str;
   str_buffer >> str;
   return str;
}

/** Convert to a string. */
template<typename T = int>
inline T
ToNumber(const std::string& _str)
{
   if constexpr(isTypeSame<T, int>())              return std::stoi(_str);
   else if constexpr(isTypeSame<T, long>())        return std::stol(_str);
   else if constexpr(isTypeSame<T, long long>())   return std::stoll(_str);
   else if constexpr(isTypeSame<T, float>())       return std::stof(_str);
   else if constexpr(isTypeSame<T, double>())      return std::stod(_str);
   else if constexpr(isTypeSame<T, long double>()) return std::stold(_str);
}

/** Check if the first string is a substring of the second. */
inline bool
isSubstring(const std::string& _inner, const std::string& _string) { return _string.find(_inner) != std::string::npos; }

/** Replace a substring with another substring, in a string. */
inline std::string
Replace(const std::string& _substr, const std::string& _replace, const std::string& _string) { return std::regex_replace(_string, std::regex{_substr}, _replace); }

/** Remove a substring from another string. */
inline std::string
Remove(const std::string& _substr, const std::string& _string) { return Replace(_substr, "", _string); }

/** Split a string into substrings given a delimiter. */
inline std::vector<std::string>
Split(std::string _input, const std::string& _delimiter = "")
{
   if(_delimiter == "")
   {
      const std::vector<char> temp(_input.begin(), _input.end());
      std::vector<std::string> outp(temp.size());
      FOR(i, temp.size()) outp[i] = temp[i];
      return outp;
   }

   size_t pos{};
   std::string sub_string;
   std::vector<std::string> output;

   while((pos = _input.find(_delimiter)) != std::string::npos)
   {
      sub_string = _input.substr(0, pos);
      output.push_back(sub_string);
      _input.erase(0, pos + _delimiter.length());
   }
   output.push_back(_input);

   return output;
}

}
