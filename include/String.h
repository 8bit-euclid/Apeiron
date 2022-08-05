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

/** Convert a string to a number. */
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

/** Peek at the character at the given position, if it lies within the bounds of the string interators. */
template<CharIterator Iter>
inline std::pair<Iter, bool>
PeekAt(unsigned position, const Iter first, const Iter last)
{
   if(position < std::distance(first, last)) return { first + position, true };
   else return { last, false };
}

/** Check if the first string is a substring of the second. */
inline bool
isSubstring(const std::string& substr, const std::string& str) { return str.find(substr) != std::string::npos; }

/** Replace a substring with another substring, in a string. */
inline std::string
Replace(const std::string& from_str, const std::string& to_str, const std::string& str)
{
   if(from_str.empty()) return str;

   std::string out_str(str);
   size_t start_pos{};
   while((start_pos = out_str.find(from_str, start_pos)) != std::string::npos)
   {
      out_str.replace(start_pos, from_str.length(), to_str);
      start_pos += to_str.length();
   }
   return out_str;
}

/** Remove a substring from another string. */
inline std::string
Remove(const std::string& substr, const std::string& str) { return Replace(substr, "", str); }

/** Split a string into substrings given a delimiter. */
inline std::vector<std::string>
Split(std::string input, const std::string& delimiter = "")
{
   if(delimiter == "")
   {
      std::vector<std::string> outp;
      outp.reserve(input.size());
      std::transform(input.begin(), input.end(), std::back_inserter(outp), [](auto& c){return std::string(1, c);});
      return outp;
   }

   size_t pos;
   std::string sub_string;
   std::vector<std::string> output;

   while((pos = input.find(delimiter)) != std::string::npos)
   {
      sub_string = input.substr(0, pos);
      output.push_back(sub_string);
      input.erase(0, pos + delimiter.length());
   }
   output.push_back(input);

   return output;
}

/** Get the bounding iterators of the first substring enclosed between the given opening and closing symbols. */
template<CharIterator Iter>
inline std::pair<Pair<Iter>, bool>
GetFirstEnclosure(const Iter first, const Iter last, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   // Find the iterator of the opening symbol.
   const auto open_it = std::find(first, last, opening_brace);
   if(open_it == last) return { { last, last }, false };

   // Find the iterator of the closing symbol.
   auto condition = [opening_brace, closing_brace, count = 0](const char c) mutable
                    {
                       if(opening_brace != closing_brace)
                       {
                          if(c == opening_brace) count++;
                          else if(c == closing_brace) count--;
                          return count <= 0;
                       }
                       else
                       {
                          if(c == opening_brace) count++;
                          return count == 2;
                       }
                    };
   const auto close_it = std::find_if(open_it, last, condition);

   ASSERT(close_it != open_it, "The closing symbol is at the same location as the opening symbol.")
   ASSERT(close_it != last, "Could not find the closing symbol in the input string.")

   return { include_braces ? Pair<Iter>{ open_it, close_it + 1 } : Pair<Iter>{ open_it + 1, close_it }, true };
}

/** Get the first substring enclosed between the given opening and closing symbols. */
inline std::pair<std::string, bool>
GetFirstEnclosure(const std::string& input, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   auto [iterators, was_found] = GetFirstEnclosure(input.begin(), input.end(), opening_brace, closing_brace, include_braces);
   return { std::string(iterators.first, iterators.second), was_found };
}

/** Get the bounding iterators of all substrings enclosed between the given opening and closing symbols, that are also chained together. */
template<CharIterator Iter>
inline std::vector<Pair<Iter>>
GetFirstEnclosureChain(Iter first, const Iter last, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   std::vector<Pair<Iter>> enclosure_chain;
   enclosure_chain.reserve(std::distance(first, last));

   bool find_next;
   do
   {
      find_next = false;
      auto [bounds, found] = GetFirstEnclosure(first, last, opening_brace, closing_brace, include_braces);
      if(found)
      {
         enclosure_chain.push_back(bounds);
         const auto trailing_iter = bounds.second + (include_braces ? 0 : 1);
         if(bounds.second != last && *trailing_iter == opening_brace)
         {
            first = trailing_iter;
            find_next = true;
         }
      }
   }
   while(find_next);

   enclosure_chain.shrink_to_fit();
   return enclosure_chain;
}

/** Get all substrings enclosed between the given opening and closing symbols, that are also chained together. */
inline std::vector<std::string>
GetFirstEnclosureChain(const std::string& input, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   auto enclosure_chain_iters = GetFirstEnclosureChain(input.begin(), input.end(), opening_brace, closing_brace, include_braces);

   std::vector<std::string> enclosure_chain;
   enclosure_chain.reserve(enclosure_chain_iters.size());

   std::transform(enclosure_chain_iters.begin(), enclosure_chain_iters.end(), std::back_inserter(enclosure_chain),
                  [](auto& p){return std::string(p.first, p.second);});

   return enclosure_chain;
}

/** Get the bounding iterators of all substrings enclosed between the given opening and closing symbols, in the order in which they appear. */
template<CharIterator Iter>
inline std::vector<Pair<Iter>>
GetAllEnclosures(Iter first, const Iter last, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   std::vector<Pair<Iter>> enclosures;
   enclosures.reserve(std::distance(first, last));
   bool found_enclosure;

   do
   {
      // Find next enclosure iterators. If found, push onto list of enclosure iterators and update the first iterator.
      Pair<Iter> bounds;
      std::tie(bounds, found_enclosure) = GetFirstEnclosure(first, last, opening_brace, closing_brace, include_braces);
      if(found_enclosure)
      {
         enclosures.push_back(bounds);
         first = bounds.second + (include_braces ? 0 : 1);
      }
   }
   while(found_enclosure);

   enclosures.shrink_to_fit();
   return enclosures;
}

/** Get all substrings enclosed between the given opening and closing symbols, in the order in which they appear. */
inline std::vector<std::string>
GetAllEnclosures(const std::string& input, const char opening_brace, const char closing_brace, const bool include_braces = false)
{
   auto enclosure_iters = GetAllEnclosures(input.begin(), input.end(), opening_brace, closing_brace, include_braces);

   std::vector<std::string> enclosures;
   enclosures.reserve(enclosure_iters.size());

   std::transform(enclosure_iters.begin(), enclosure_iters.end(), std::back_inserter(enclosures),
                  [](auto& p){return std::string(p.first, p.second);});

   return enclosures;
}

}
