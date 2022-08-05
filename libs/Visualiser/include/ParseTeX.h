#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Glyph.h"

#include <string>

namespace aprn::vis {

/***************************************************************************************************************************************************************
* TeX Parsing Functions
***************************************************************************************************************************************************************/
template<CharIterator Iter>
DArray<Glyph>
ParseTeXString(Iter& current, const Iter last, const bool is_math_mode = false);

Glyph ParseTeXChar(const char c);

template<CharIterator Iter>
Glyph ParseTeXGlyph(Iter& current, const Iter last, const bool is_math_mode = false);

template<CharIterator Iter>
Glyph ParseTeXCommand(Iter& current, const Iter last, const bool is_math_mode = false);

template<CharIterator Iter>
DArray<Glyph>
ParseTeXMath(Iter& current, const Iter last);

template<CharIterator Iter>
std::pair<DArray<Glyph>, std::string>
ParseAllTeXScriptText(Iter& current, const Iter last);

/***************************************************************************************************************************************************************
* TeX Parsing Helper Functions
***************************************************************************************************************************************************************/
size_t CountGlyphChars(const Glyph& glyph);

bool isGlyphString(const std::string& tex_str);

template<CharIterator Iter>
bool isTeXCommandEnd(const Iter current, const Iter last);

template<CharIterator Iter>
std::pair<bool, Iter>
isTeXCharCommand(const Iter first, const Iter last);

template<CharIterator Iter>
std::pair<bool, Iter>
isTeXWordCommand(const Iter first, const Iter last);

template<CharIterator Iter>
std::tuple<Iter, bool, bool>
GetTeXCommandInfo(const Iter current, const Iter last);

template<CharIterator Iter>
DArray<Glyph>
ParseTeXScriptText(Iter& current, const Iter last);

template<CharIterator Iter>
Iter GetTeXCommandPrefixEnd(const Iter first, const Iter last, const bool is_char_cmd);

}

#include "ParseTeX.tpp"
