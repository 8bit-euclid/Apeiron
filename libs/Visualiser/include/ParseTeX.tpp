#pragma once

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeX Parsing Functions
***************************************************************************************************************************************************************/
template<CharIterator Iter>
DArray<Glyph>
ParseTeXString(Iter& current, const Iter last, const bool is_math_mode)
{
   DArray<Glyph> glyphs;
   glyphs.reserve(std::distance(current, last));

   while(current < last)
   {
      const auto c = *current;
      if(c != '\\')
      {
         if(c != '$') glyphs.Append(ParseTeXGlyph(current, last, is_math_mode)); // TeX single/compound character
         else if(!is_math_mode) glyphs.Append(ParseTeXMath(current, last)); // TeX math mode
         else EXIT("This code should not be reachable if already in math mode.")
      }
      else glyphs.Append(ParseTeXCommand(current, last)); // TeX command
   }

   ASSERT(current == last, "Could not parse the TeX string correctly.")
   glyphs.shrink_to_fit();
   return glyphs;
}

template<CharIterator Iter>
Glyph
ParseTeXGlyph(Iter& current, const Iter last, const bool is_math_mode)
{
   if(!is_math_mode) return ParseTeXChar(*(current++));
   else
   {
      // Parse single character and check for trailing superscripts/subscripts.
      Glyph glyph = ParseTeXChar(*(current++));
      if(glyph.isRendered() && *current == OneOf('_', '^'))
      {
         auto [sub_glyphs, script_text] = ParseAllTeXScriptText(current, last);
         glyph.Add(Glyph(glyph.GetText())); // Need to add base glyph as a subglyph first.
         glyph.Add(std::move(sub_glyphs));
         glyph.Add(std::move(script_text));
      }
      return glyph;
   }
}

template<CharIterator Iter>
Glyph
ParseTeXCommand(Iter& current, const Iter last, const bool is_math_mode)
{
   ASSERT(*current == '\\', "A TeX command must begin with a backslash character.")

   const auto [cmd_prefix_end, has_args, render] = GetTeXCommandInfo(current, last);
   const auto cmd = std::string(current, cmd_prefix_end);
   const auto trailing_char = cmd_prefix_end != last ? *cmd_prefix_end : '\0';

   Glyph glyph;
   glyph.Set(cmd);
   if(!render) glyph.DoNotRender();
   current = cmd_prefix_end;

   if(has_args)
   {
      if(trailing_char != OneOf('_', '^'))
      {
         ASSERT(trailing_char == '{', "Expected at least one argument after the command ", cmd, ", but there is no trailing { brace.")

         const auto enclosures = GetFirstEnclosureChain(cmd_prefix_end, last, '{', '}', true);
         const int  n_args     = enclosures.size();
         ASSERT(n_args == OneOf(1, 2), "Expected either one or two arguments for command ", cmd, ", but none were found.")

         // Parse enclosures and add all items as sub-glyphs.
         FOR_EACH(encl, enclosures)
         {
            std::string encl_str;
            auto sub_glyphs = ParseTeXString(current = encl.first + 1, encl.second - 1, is_math_mode);

            // Add opening brace, subglyph text, and closing brace.
            encl_str += '{';
            FOR_EACH(glyph, sub_glyphs) encl_str += glyph.GetText();
            encl_str += '}';

            glyph.Add(std::move(sub_glyphs));
            glyph.Add(std::move(encl_str));
         }
         ++current;
      }
      else
      {
         ASSERT(trailing_char == OneOf('_', '^'),
                "Expected to parse a superscript/subscript but got the following trailing character: ", std::string{trailing_char}, ".")

         // Check for subscripts/superscripts. If there are any, add them as sub-glyphs of this glyph.
         auto [sub_glyphs, script_text] = ParseAllTeXScriptText(current, last);
         glyph.Add(Glyph(glyph.GetText())); // Need to add base glyph as a subglyph first.
         glyph.Add(std::move(sub_glyphs));
         glyph.Add(std::move(script_text));
      }
   }

   return glyph;
}

template<CharIterator Iter>
DArray<Glyph>
ParseTeXMath(Iter& current, const Iter last)
{
   ASSERT(*current == '$', "In math mode, the first character must point to the $ symbol.")

   DArray<Glyph> glyphs;
   glyphs.reserve(std::distance(current, last));

   const auto brace_glyph = ParseTeXChar('$');
   const bool is_double_braced = *(current + 1) == '$';

   // Add opening math-mode brace/s.
   glyphs.Append(brace_glyph);
   if(is_double_braced) glyphs.Append(brace_glyph);

   // Find the bounding iterators of the enclosing math-mode string.
   if(is_double_braced) ++current;
   const auto [bounds, found] = GetFirstEnclosure(current, last, '$', '$');
   ASSERT(found, "Could not find an enclosed substring in math mode.")

   // Add glyphs from the enclosing string.
   current = bounds.first;
   glyphs.Append(ParseTeXString(current, bounds.second, true));
   ASSERT(current == bounds.second, "Could not correctly parse the enclosed substring in math mode.")

   // Add closing math-mode brace/s.
   glyphs.Append(brace_glyph);
   ++current;
   if(is_double_braced)
   {
      ASSERT(*(bounds.second + 1) == '$', "Expected a second $ symbol, but did not find one.")
      glyphs.Append(brace_glyph);
      ++current;
   }

   glyphs.shrink_to_fit();
   return glyphs;
}

template<CharIterator Iter>
std::pair<DArray<Glyph>, std::string>
ParseAllTeXScriptText(Iter& current, const Iter last)
{
   std::string script_str;
   DArray<Glyph> glyphs;
   glyphs.reserve(2);

   FOR(i, 2)
      if(*current == OneOf('_', '^'))
      {
         const bool is_braced = *(current + 1) == '{';

         // Add underscore/carat and opening brace for sub-glyph insertion.
         script_str += *current;
         if(is_braced) script_str += '{';

         // Add subglyphs.
         auto script_glyphs = ParseTeXScriptText(current, last);
         FOR_EACH(glyph, script_glyphs) script_str += glyph.GetText();

         // Add closing brace and append subglyphs.
         if(is_braced) script_str += '}';
         glyphs.Append(std::move(script_glyphs));
      }

   glyphs.shrink_to_fit();
   return { glyphs, script_str };
}

/***************************************************************************************************************************************************************
* TeX Parsing Helper Functions
***************************************************************************************************************************************************************/
template<CharIterator Iter>
bool
isTeXCommandEnd(const Iter current, const Iter last)
{
   if(current == last) return true;
   const auto c = *current;
   return c == OneOf(' ', '.', ',', ';', ':', '(', '[', '\t', '\n', '\\') || std::isdigit(c) ||
          (!std::isalpha(*(current - 1)) && std::isalpha(c)); // Note: assumes base string has at least one char before current iterator.
}

template<CharIterator Iter>
std::pair<bool, Iter>
isTeXCharCommand(const Iter first, const Iter last)
{
   ASSERT(*first == '\\', "A TeX command must begin with a backslash character.")

   const bool is_char_cmd = std::distance(first, last) > 1 && !std::isdigit(*(first + 1)) &&
                           (std::distance(first, last) > 2 ? isTeXCommandEnd(first + 2, last) : true);
   const auto end_iter    = is_char_cmd ? GetTeXCommandPrefixEnd(first, last, true) : last;

   return { is_char_cmd, end_iter };
}

template<CharIterator Iter>
std::pair<bool, Iter>
isTeXWordCommand(const Iter first, const Iter last)
{
   ASSERT(*first == '\\', "A TeX command must begin with a backslash character.")

   const bool is_word_cmd = std::distance(first, last) > 2 &&
                            std::all_of(first + 1, first + 3, [](char c){ return std::isalpha(c); }); // At least two letters in the command name
   const auto end_iter    = is_word_cmd ? GetTeXCommandPrefixEnd(first, last, false) : last;

   return { is_word_cmd, end_iter };
}

template<CharIterator Iter>
std::tuple<Iter, bool, bool>
GetTeXCommandInfo(const Iter current, const Iter last)
{
   ASSERT(*current == '\\', "A TeX command must begin with a backslash character.")
   ASSERT(std::distance(current, last) > 1, "A TeX command must comprise at least two characters.")

   bool char_cmd{}, word_cmd{};
   Iter cmd_prefix_end;
   std::tie(char_cmd, cmd_prefix_end)               = isTeXCharCommand(current, last);
   if(!char_cmd) std::tie(word_cmd, cmd_prefix_end) = isTeXWordCommand(current, last);
   ASSERT(char_cmd != word_cmd, "Expected either a command character or a command word, but got ", std::string(current, last), ".")

   const auto cmd_prefix = std::string(current + 1, cmd_prefix_end); // Command name without the backslash

   if(char_cmd) // Single characters - no trailing args, and render if not a spacing character
   {
      const bool not_spacing_char = cmd_prefix != OneOf("!", ",", ":", ">", ";", " ");
      return { cmd_prefix_end, false, not_spacing_char };
   }
   else if(isTeXCommandEnd(cmd_prefix_end, last)) // Word command without trailing arguments
   {
      const bool not_spacing_word = cmd_prefix != OneOf("quad", "qquad");
      return { cmd_prefix_end, false, not_spacing_word };
   }
   else // Word command with trailing arguments
   {
      const bool not_spacing_word = cmd_prefix != OneOf("dec", "decc", "deccc", "inc", "incc", "inccc",
                                                        "vdec", "vdecc", "vdeccc", "vinc", "vincc", "vinccc");
      return { cmd_prefix_end, true, not_spacing_word };
   }
}

template<CharIterator Iter>
DArray<Glyph>
ParseTeXScriptText(Iter& current, const Iter last)
{
   ASSERT(*current == OneOf('_', '^'), "TeX superscript/subscript text must begin with either an underscore or a carat.")

   DArray<Glyph> glyphs;
   glyphs.reserve(std::distance(current, last));

   const auto next = current + 1;
   ASSERT(next != last, "Incomplete TeX string.")
   const auto next_char = *next;

   if(next_char == '{') // Enclosed string
   {
      const auto [bounds, found] = GetFirstEnclosure(current, last, '{', '}');
      ASSERT(found, "Could not find an enclosed substring in the subscript/superscript.")

      // Add glyphs from the enclosed string.
      current = bounds.first;
      glyphs.Append(ParseTeXString(current, bounds.second, true));
      ASSERT(current == bounds.second, "Could not correctly parse the enclosed substring in the subscript/superscript.")
      ++current; // Advance one past the closing brace
   }
   else // Single character
   {
      glyphs.emplace_back(next_char);
      current += 2;
   }

   glyphs.shrink_to_fit();
   return glyphs;
}

template<CharIterator Iter>
Iter GetTeXCommandPrefixEnd(const Iter first, const Iter last, const bool is_char_cmd)
{
   DEBUG_ASSERT(*first == '\\', "A TeX command must begin with a backslash character.")
   return is_char_cmd ? first + 2 : std::find_if_not(first + 1, last, [](auto c){ return std::isalpha(c); });
}

}