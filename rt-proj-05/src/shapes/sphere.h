#ifndef SPHERE_H
#define SPHERE_H

#include "../core/shape.h"

namespace rt3 {

class Sphere : public Shape {
public:
    Point3f center;
    real_type radius;
 
    Sphere(Point3f ori, real_type r): center(ori), radius(r) {}

    ~Sphere(){}

    real_type calc_delta(const Ray &r, real_type &A, real_type &B) const;

    bool intersect_p(const Ray &r, real_type maxT ) const override;
    bool intersect(const Ray &r, shared_ptr<Surfel> &isect) const override;
};


Sphere *create_sphere(const ParamSet &ps);

}

#endif