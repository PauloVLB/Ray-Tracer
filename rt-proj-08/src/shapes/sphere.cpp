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
        //std::cout << "oi???" << std::endl;
        
        real_type A, B;
        real_type delta = calc_delta(r, A, B);
        if(delta >= 0){
            //std::cout << "delta >= 0" << std::endl;
            real_type t[2] = {
                (-B - (real_type)sqrt(delta)) / (2 * A),
                (-B + (real_type)sqrt(delta)) / (2 * A),
            };

            if(t[0] > t[1]) std::swap(t[0], t[1]);

            Point3f contact;
            
            real_type cont_t = t[1];
            if(t[0] > 0) {
                contact = r(t[0]);
                cont_t = t[0];
            }
            else if(t[1] > 0) {
                contact = r(t[1]);
                cont_t = t[1];
            }
            else return false;

            Vector3f normal = glm::normalize(contact - center);

            isect = shared_ptr<Surfel>(new Surfel(
                contact /* + normal * 0.001f */,
                normal,
                -r.d,
                cont_t
            ));

            return true;
        }else{
            //std::cout << "delta < 0" << std::endl;
            return false;
        }
    }

    bool Sphere::intersect_p(const Ray& r, real_type maxT ) const {
        real_type A, B;
        real_type delta = calc_delta(r, A, B);

        if(delta >= 0){
            real_type t[2] = {
                (-B - (real_type)sqrt(delta)) / (2 * A),
                (-B + (real_type)sqrt(delta)) / (2 * A),
            };
            if(t[0] > t[1]) std::swap(t[0], t[1]);
            if(t[0] > 0) return t[0] < maxT;
            else if(t[1] > 0) return t[1] < maxT;
            else return false;
        } else return false;
    }

    Bounds3f Sphere::computeBounds() const{
        Point3f radiusPoint{radius, radius, radius};

        return Bounds3f{center - radiusPoint, center + radiusPoint};
    }

    Sphere *create_sphere(const ParamSet &ps, shared_ptr<Transform> tr) {
        return new Sphere(
            retrieve(ps, "center", Point3f()),
            retrieve(ps, "radius", real_type(1)),
            tr
        );
    }
}