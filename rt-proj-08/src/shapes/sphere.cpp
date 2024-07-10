#include "sphere.h"
#include "../core/material.h"

namespace rt3 {

    real_type Sphere::calc_delta(const Ray &r, real_type &A, real_type &B) const {
        Vector3f oc = (r.o - center);
        A = glm::dot(r.d, r.d);
        B = 2 * glm::dot(oc, r.d);
        real_type C = glm::dot(oc, oc) - (radius * radius);
        real_type delta = (B*B) - (4*A*C);

        return delta;
    }

    bool Sphere::intersect(const Ray &r, shared_ptr<Surfel> &isect) const {
        auto invRay = inv_transform->apply_r(r);

        real_type t;
        if(!calc_t(invRay, t)) return false;
            
        Point3f contact = invRay(t);
        Normal3f normal = glm::normalize(contact - center);

            contact = transform->apply_p(contact);
            t = glm::length(contact - r.o);

        isect = unique_ptr<Surfel>(new Surfel(
            transform->apply_p(contact), // contact point
            transform->apply_n(normal),
            -r.d, // original ray dir
            t // t
        ));
        return true;
    }

    bool Sphere::calc_t(const Ray &r, real_type &t) const {
        real_type A, B;
        real_type delta = calc_delta(r, A, B);

        if(delta >= 0){
            real_type tt[2] = {
                (-B - (real_type)sqrt(delta)) / (2 * A),
                (-B + (real_type)sqrt(delta)) / (2 * A),
            };
            if(tt[0] > tt[1]) std::swap(tt[0], tt[1]);
            if(tt[0] < 0){
                if(tt[1] < 0) return false;
                else{
                    t = tt[1];
                    return true;
                }
            }else{
                t = tt[0];
                return true;
            }
        } else return false;
    }

    bool Sphere::intersect_p(const Ray& r, real_type maxT ) const {
        auto transformed_ray = inv_transform->apply_r(r);
        real_type t;
        if(!calc_t(transformed_ray, t)) return false;

            Point3f contact = transformed_ray(t);
            contact = transform->apply_p(contact);
            t = glm::length(contact - r.o);

        return t < maxT;
    }

    Bounds3f Sphere::computeBounds() const{
        Point3f radiusPoint{radius, radius, radius};

        return transform->apply_b(Bounds3f{center - radiusPoint, center + radiusPoint});
    }

    Sphere *create_sphere(const ParamSet &ps, shared_ptr<Transform> tr) {
        return new Sphere(
            retrieve(ps, "center", Point3f()),
            retrieve(ps, "radius", real_type(1)),
            tr
        );
    }
}