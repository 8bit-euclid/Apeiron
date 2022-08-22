/***************************************************************************************************************************************************************
* GPL-3.0 License
* Copyright (C) 2022 Niran A. Ilangakoon
*
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************************************************************************/

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
bool isGlyphString(const std::string_view& tex_str);

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
