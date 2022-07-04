#pragma once

#include "Types.h"

#include <type_traits>
#include <concepts>

namespace aprn {

/***************************************************************************************************************************************************************
* Type Concepts
***************************************************************************************************************************************************************/
template<typename T> concept Enumerator    = isEnum<T>();
template<typename T> concept Boolean       = isBoolean<T>();
template<typename T> concept Integral      = isIntegral<T>();
template<typename T> concept FloatingPoint = isFloatingPoint<T>();
template<typename T> concept Arithmetic    = isArithmetic<T>();
template<typename T> concept StringLiteral = isStringLiteral<T>();

}
