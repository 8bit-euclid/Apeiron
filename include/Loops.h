#pragma once

namespace aprn{

/** For loop through dimensions. */
#define FOR1(_ic) for(size_t _ic = 0; _ic < Dim; ++_ic)

/** For loop from the first index. */
#define FOR2(_index, _n_index) for(size_t _index = 0; _index < _n_index; ++_index)

/** For loop from a starting index. */
#define FOR3(_index, _start_index, _n_index) for(size_t _index = _start_index; _index < _n_index; ++_index)

/** Generic for loop. */
#define FOR(...) MACRO(FOR, __VA_ARGS__)

/** For loop in reverse from a starting index to the first index. */
#define FOR_REVERSE2(_index, _start_index) for(size_t _index = _start_index; _index >= 0; --_index)

/** For loop in reverse from a starting index. */
#define FOR_REVERSE3(_index, _start_index, _end_index) for(size_t _index = _start_index; _index >= _end_index; --_index)

/** Generic for loop in reverse. */
#define FOR_REVERSE(...) MACRO(FOR_REVERSE, __VA_ARGS__)

/** For loop over the reference to each item in a list. */
#define FOR_EACH(_item, _list) for(auto& _item : (_list))

}//aprn
