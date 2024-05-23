#include "color.h"

namespace rt3 {

Color Color::operator * (const real_type t) const { 
    return {(r*t), (g*t), (b*t)}; 
}

Color Color::operator * (const Color c) const {
    return {(r*c.r), (g*c.g), (b*c.b)};
}

Color Color::operator + (const Color c) const { 
    return {(r+c.r), (g+c.g), (b+c.b)}; 
}

}