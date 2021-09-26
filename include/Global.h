#ifndef GLOBAL_H
#define GLOBAL_H

#include "StandardLibrary.h"

/** Run-time macros. */
//#define PARALLEL

/** Debugging macros. */
#define DEBUG_MODE
#define STACK_TRACE_SIZE 10

/** Dimension. */
constexpr int nDim(2);

namespace Apeiron
{

#include "Macros.h"
#include "FloatingPoint.h"
#include "Loops.h"
#include "Print.h"
#include "Debug.h"

}//Apeiron

#endif //GLOBAL_H
