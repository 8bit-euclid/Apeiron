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

#include <string>

namespace aprn::vis {

class TeXObject
{
 public:
   virtual ~TeXObject() = default;

   virtual TeXObject& SetColour(const SVectorR4& rgba_colour) = 0;

   virtual TeXObject& SetColour(const Colour& colour) = 0;

   virtual TeXObject& SetItalic(bool is_italic) = 0;

   virtual TeXObject& SetBold(bool is_bold) = 0;

   inline const std::string& Text() const { return _Text; }

 protected:
   std::string _Text;
};

}
