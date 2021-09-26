#ifndef MACROS_H
#define MACROS_H

/** Macro expansions. */
#define EXPAND(x) x
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define ARG_COUNT(...) EXPAND(PP_ARG_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/** General definition for any macro name */
#define _MACRO_(name, n) name##n
#define _MACRO(name, n) _MACRO_(name, n)
#define MACRO(func, ...) _MACRO(func, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#endif //MACROS_H
