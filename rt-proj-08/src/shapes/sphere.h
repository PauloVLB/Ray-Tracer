#ifndef SPHERE_H
#define SPHERE_H

#include "../core/shape.h"
#include "../core/transform.h"

namespace rt3 {

class Sphere : public Shape {
public:
    Point3f center;
    real_type radius;
 
    Sphere(Point3f cen, real_type r): center(cen), radius(r) {}
    Sphere(Point3f cen, real_type r, shared_ptr<Transform> t): 
        center(cen), radius(r), 
        transform(t), inv_transform(make_shared<Transform>(transform->inverse())) {}

    ~Sphere(){}

    real_type calc_delta(const Ray &r, real_type &A, real_type &B) const;

    Bounds3f computeBounds() const override;

    bool intersect_p(const Ray &r, real_type maxT ) const override;
    bool intersect(const Ray &r, shared_ptr<Surfel> &isect) const override;

    bool calc_t(const Ray &r, real_type &t) const;

    shared_ptr<Transform> transform, inv_transform;
};


Sphere *create_sphere(const ParamSet &ps, shared_ptr<Transform> tr);

} // namespace rt3'


#endif