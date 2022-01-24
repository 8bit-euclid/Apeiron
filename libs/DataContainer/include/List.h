#pragma once

#include "../include/Global.h"

#include <list>

namespace Apeiron {

template <typename T>
class List : public std::list<T>
{
public:
  List() = default;
  ~List() = default;

  /** Size and Index Range-checking ***************************************************************************************************************************/
  void IndexBoundCheck(const std::size_t _index) const
  {
    DEBUG_ASSERT(this->size(), "The list has not yet been sized.")
    DEBUG_ASSERT(isBounded(_index, std::size_t(0), this->size()), "The list index ", _index, " must be in the range [0, ", this->size() - 1, "].")
  }

  void SizeCheck(const std::size_t _size0, const std::size_t _size1) const
  {
    DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The list sizes ", _size0, " and ", _size1, " must be equal.")
  }

  /** Subscript Operator Overloads ****************************************************************************************************************************/
  T& operator[](const std::size_t _index)
  {
    IndexBoundCheck(_index);
    auto iterator = this->begin();
    FOR(i, _index) ++iterator;
    return *iterator;
  }

  const T& operator[](const std::size_t _index) const
  {
    IndexBoundCheck(_index);
    auto iterator = this->begin();
    FOR(i, _index) ++iterator;
    return *iterator;
  }

  /** Assignment Operator Overloads ***************************************************************************************************************************/
  List& operator=(const T& _value) noexcept
  {
    FOR_EACH(entry, *this) entry = _value;
    return *this;
  }

  List& operator=(const std::initializer_list<T>& _value_list)
  {
    SizeCheck(_value_list.size(), this->size());
    std::size_t index(0);
    FOR_EACH(entry, _value_list) (*this)[index++] = entry;
    return *this;
  }
};

}
