#include "primitive.h"

namespace rt3 {

bool PrimList::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    for(auto &prim : primitives) {
        if(prim->intersect(r, isect)) {
            return true;
        }
    }
    return false;
}

bool PrimList::intersect_p(const Ray& r) const {
    for(auto &prim : primitives) {
        if(prim->intersect_p(r)) return true;
    }
    return false;
}

bool GeometricPrimitive::intersect_p( const Ray& r ) const {
    return shape->intersect_p(r); 
}

bool GeometricPrimitive::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    /* std::cout << "oi" << std::endl;
    return shape->intersect(r, isect); */
    if(shape->intersect(r, isect)){
        auto shared_this = shared_ptr<const GeometricPrimitive>(shared_from_this());
        isect->primitive = shared_this;
        return true;
    } else {
        return false; 
    }
}


}