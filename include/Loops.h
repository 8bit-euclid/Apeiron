#pragma once

namespace aprn{

/***************************************************************************************************************************************************************
* Specialised Loop Definitions
***************************************************************************************************************************************************************/
namespace detail {

/** Ordinary for loops
***************************************************************************************************************************************************************/
#define FOR2(index, count) for(size_t index = 0; index < count; ++index)

#define FOR3(index, start_index, count) for(size_t index = start_index; index < count; ++index)

/** Reverse for loops
***************************************************************************************************************************************************************/
#define FOR_REVERSE2(index, start_index) for(size_t index = start_index; index >= 0; --index)

#define FOR_REVERSE3(index, start_index, end_index) for(size_t index = start_index; index >= end_index; --index)

/** Non-const range for loops
***************************************************************************************************************************************************************/
#define FOR_EACH2(item, list) for(auto& item : (list))

#define FOR_EACH3(key, value, map) for(auto& [key, value] : (map))

/** Const range for loops
***************************************************************************************************************************************************************/
#define FOR_EACH_CONST2(item, list) for(const auto& item : (list))

#define FOR_EACH_CONST3(key, value, map) for(const auto& [key, value] : (map))

}

/***************************************************************************************************************************************************************
* Generic Loop Definitions
***************************************************************************************************************************************************************/
#define FOR(...) MACRO(FOR, __VA_ARGS__)

#define FOR_REVERSE(...) MACRO(FOR_REVERSE, __VA_ARGS__)

#define FOR_ITER(it, begin, end) for(auto it = begin; it != end; ++it)

#define FOR_EACH(...) MACRO(FOR_EACH, __VA_ARGS__)

#define FOR_EACH_CONST(...) MACRO(FOR_EACH_CONST, __VA_ARGS__)

}//aprn
