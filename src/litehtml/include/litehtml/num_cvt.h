#ifndef NUM_CVT_H
#define NUM_CVT_H

#include <string>

#include "string.h"

namespace litehtml {
namespace num_cvt {
std::string to_latin_lower(int val);
std::string to_latin_upper(int val);
std::string to_greek_lower(int val);
std::string to_roman_lower(int value);
std::string to_roman_upper(int value);
} // namespace num_cvt
} // namespace litehtml

#endif // NUM_CVT_H
