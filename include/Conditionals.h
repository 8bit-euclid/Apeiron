#pragma once

#include <algorithm>
#include <tuple>

namespace aprn {

/***************************************************************************************************************************************************************
* Conditional Helper Functions
***************************************************************************************************************************************************************/
namespace detail {

/** OneOf helper struct for a variable number of arguments. */
template<typename ...Ts>
struct VariadicOneOfHelper
{
   constexpr VariadicOneOfHelper(Ts... values) : Values(std::move(values)...) {}

   template<typename T>
   [[nodiscard]] friend constexpr bool
   operator==(T lhs, const VariadicOneOfHelper& rhs) noexcept { return std::apply([&](auto... vals){ return ((lhs == vals) || ...); }, rhs.Values); }

   template<typename T>
   [[nodiscard]] friend constexpr bool
   operator!=(T lhs, const VariadicOneOfHelper& rhs) noexcept { return !operator==(lhs, rhs); }

   std::tuple<Ts...> Values;
};

/** OneOf helper struct for entries of an STL container type. */
template<typename Cont>
struct ContainerOneOfHelper
{
   constexpr ContainerOneOfHelper(Cont const& values) : Values(values) {}

   template<typename T>
   [[nodiscard]] friend constexpr bool
   operator==(T&& lhs, ContainerOneOfHelper&& rhs) noexcept { return std::any_of(cbegin(rhs.Values), cend(rhs.Values), [&](auto val){ return lhs == val; }); }

   template<typename T>
   [[nodiscard]] friend constexpr bool
   operator!=(T&& lhs, ContainerOneOfHelper&& rhs) noexcept { return !operator==(std::move(lhs), std::move(rhs)); }

   const Cont& Values;
};

template<typename T>
auto isContainer(int) -> decltype(cbegin(std::declval<T>()) == cend(std::declval<T>()), std::true_type{});

template<typename T>
std::false_type isContainer(...);

template<typename T>
constexpr bool isContainer_v = decltype(isContainer<T>(0))::value;

}//detail

/***************************************************************************************************************************************************************
* Conditionals
***************************************************************************************************************************************************************/

/** Init for equality with 'any of' the entries in a list of arguments or entries in an STL container. */
template<typename ...Ts>
[[nodiscard]] constexpr auto
OneOf(Ts&&... values)
{
   using namespace detail;
   if constexpr(sizeof...(Ts) == 1 && isContainer_v<std::tuple_element_t<0, std::tuple<Ts...>>>) return ContainerOneOfHelper(std::forward<Ts>(values)...);
   else return VariadicOneOfHelper(std::forward<Ts>(values)...);
}

}
