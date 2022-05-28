#ifndef ARRAY_TEMPLATE_DEF
#define ARRAY_TEMPLATE_DEF

#include "../include/Array.h"
#include "../include/Detail.h"

namespace aprn {

/***************************************************************************************************************************************************************
* Array Abstract Base Class
***************************************************************************************************************************************************************/

/** Size and Index Range-checking */
template<typename T, class derived>
constexpr void
Array<T, derived>::IndexBoundCheck(const size_t _index) const
{
  DEBUG_ASSERT(!Derived().empty(), "The array has not yet been sized.")
  DEBUG_ASSERT(isBounded(_index, size_t(0), Derived().size()), "The array index ", _index, " must be in the range [0, ", Derived().size() - 1, "].")
}

template<typename T, class derived>
constexpr void
Array<T, derived>::SizeCheck(const size_t _size0, const size_t _size1) const
{
  DEBUG_ASSERT(areSizesEqual(_size0, _size1), "The array sizes ", _size0, " and ", _size1, " must be equal.")
}

/** Subscript Operator Overloads */
template<typename T, class derived>
constexpr T&
Array<T, derived>::operator[](const size_t _index)
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

template<typename T, class derived>
constexpr const T&
Array<T, derived>::operator[](const size_t _index) const
{
  IndexBoundCheck(_index);
  return *(Derived().begin() + _index);
}

/** Assignment Operator Overloads */
template<typename T, class derived>
constexpr derived&
Array<T, derived>::operator=(const std::convertible_to<T> auto _value) noexcept
{
  FOR_EACH(entry, Derived()) entry = static_cast<T>(_value);
  return Derived();
}

template<typename T, class derived>
constexpr derived&
Array<T, derived>::operator=(const std::initializer_list<T>& _value_list) noexcept
{
  SizeCheck(_value_list.size(), Derived().size());
  size_t index(0);
  FOR_EACH(entry, _value_list) Derived()[index++] = entry;
  return Derived();
}

/** Comparison Operator Overloads */
template<typename T, class derived>
constexpr bool Array<T, derived>::operator==(const Array<T, derived>& _other) noexcept
{
  if(_other.Derived().size() != Derived().size()) return false;
  FOR(i, _other.Derived().size()) if(Derived()[i] != _other[i]) return false;
  return true;
}

template<typename T, class derived>
constexpr bool Array<T, derived>::operator!=(const Array<T, derived>& _other) noexcept { return !(Derived() == _other); }

/** Non-member functions */
template<typename T, class derived>
std::ostream&
operator<<(std::ostream& _output_stream, const Array<T, derived>& _array_base)
{
  const derived& derived_class = static_cast<const derived&>(_array_base);

  _output_stream << "(";
  FOR(i, derived_class.size()) _output_stream << derived_class[i] << (i == derived_class.size() - 1 ? "" : ", ");
  _output_stream << ")";

  return _output_stream;
}

/***************************************************************************************************************************************************************
* Static Array Class
***************************************************************************************************************************************************************/
template<typename T, size_t size>
constexpr StaticArray<T, size>::StaticArray()
  : StaticArray(GetStaticInitValue<T>()) {}

template<typename T, size_t size>
constexpr StaticArray<T, size>::StaticArray(const T& _value)
  : std::array<T, size>(detail::InitStaticArray<T, size>(_value)) {}

template<typename T, size_t size>
constexpr StaticArray<T, size>::StaticArray(const std::initializer_list<T>& _list)
  : std::array<T, size>(detail::InitStaticArray<T, size>(_list))
{
  ASSERT(size == _list.size(), "The initializer list should be of size ", size, ".")
}

template<typename T, size_t size>
template<class iter>
constexpr StaticArray<T, size>::StaticArray(const iter _first, const iter _last)
  : std::array<T, size>(detail::InitStaticArray<T, size>(_first, _last))
{
  STATIC_ASSERT((isTypeSame<T, typename std::iterator_traits<iter>::value_type>()), "Mismatch in the iterator data type.")
  ASSERT(size == std::distance(_first, _last), "The number of iterators must equal the array size ", size, ".")
}

/***************************************************************************************************************************************************************
* Dynamic Array Class
***************************************************************************************************************************************************************/
template<typename T>
DynamicArray<T>::DynamicArray()
  : std::vector<T>() {}

template<typename T>
DynamicArray<T>::DynamicArray(const size_t _size)
  : DynamicArray(_size, GetDynamicInitValue<T>()) {}

template<typename T>
DynamicArray<T>::DynamicArray(const size_t _size, const T& _value)
  : std::vector<T>(_size, _value) {}

template<typename T>
DynamicArray<T>::DynamicArray(const std::initializer_list<T>& _list)
  : std::vector<T>(_list) {}

template<typename T>
template<class iter>
DynamicArray<T>::DynamicArray(const iter _first, const iter _last)
  : std::vector<T>(_first, _last) {}

}

#endif //ARRAY_TEMPLATE_DEF

