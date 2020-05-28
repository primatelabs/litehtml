#ifndef LITEHTML_CSS_NUMBER_H__
#define LITEHTML_CSS_NUMBER_H__

#include "litehtml/types.h"

namespace litehtml {

enum css_number_value_type {
    kCSSIntegerValue,
    kCSSNumberValue,
};

class css_number {
    css_number_value_type type_;
    double value_;

public:
    css_number()
    : type_(kCSSIntegerValue)
    , value_(0.0)
    {
    }

    css_number(css_number_value_type type, double value)
    : type_(type)
    , value_(value)
    {
    }

    css_number_value_type type() const
    {
        return type_;
    }

    double value() const
    {
        return value_;
    }
};

} // namespace litehtml

#endif // LITEHTML_CSS_NUMBER_H__
