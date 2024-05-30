#include "color.h"

namespace rt3 {

Color cap(const Color &c) {
    return {std::min(1.0f, std::max(0.0f, c.r)), std::min(1.0f, std::max(0.0f, c.g)), std::min(1.0f, std::max(0.0f, c.b))};
}

Color Color::operator * (const real_type t) const { 
    return cap({(r*t), (g*t), (b*t)}); 
}

Color Color::operator * (const Color c) const {
    return cap({(r*c.r), (g*c.g), (b*c.b)});
}

Color Color::operator + (const Color c) const { 
    return cap({(r+c.r), (g+c.g), (b+c.b)}); 
}

}