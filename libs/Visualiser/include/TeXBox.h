#pragma once

#include "../../../include/Global.h"
#include "DataContainer/include/Array.h"
#include "Glyph.h"
#include "Model.h"

namespace aprn::vis {

class Word : public Model
{
 public:

 private:
   std::unordered_map<std::string, SPtr<Glyph>> Glyphs;
};

class Phrase : public Model
{
 public:

 private:
   std::unordered_map<std::string, SPtr<Word>> Words;
};

class TeXBox : public Model
{
 public:

 private:
   std::unordered_map<std::string, SPtr<Phrase>> Phrases;
};

}
