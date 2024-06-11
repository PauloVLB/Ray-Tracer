#include "ray.h"

namespace rt3
{

void Ray::norm() { // Refactor for normalize from glm 
    real_type t = d.length(); // Check vector3f
    if (t == 0) {
        std::cout << "--> The vector provided it is unsuitable (zero length)";
        return; 
    }

    d = d / t;

}
    
} // namespace rt3


