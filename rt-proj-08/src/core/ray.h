#ifndef RAY_H
#define RAY_H

#include "rt3.h"
//#include "rt3-base.h"

namespace rt3 {
//class Primitive;

class Ray {
    public:
        Point3f o; //!< origin
        Vector3f d; //!< direction
        mutable real_type t_min, t_max; //!< parameters
        // ... the methods goes here
        Point3f operator()(real_type t) const { return o + d * t; }
    
        Ray (const Point3f& o, const Vector3f& d,
            real_type start=0, real_type end=INFINITY) : o{o}, d{glm::normalize(d)},
            t_min{start}, t_max{end} {/*empty*/}
        Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/}

        void norm();
        
        friend std::ostream& operator<<(std::ostream& os, const Ray& r)
        {   
            os << "Origin: { ";
            for(int k = 0; k < 3; k++) {
            os << r.o[k] << " ";
            }
            os << "} ";
            os << "Direction: { ";
            for(int k = 0; k < 3; k++) {
            os << r.d[k] << " ";
            }
            os << "}";
            return os;
        }
};



}

#endif