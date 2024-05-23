#include "primitive.h"

namespace rt3 {

bool PrimList::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    shared_ptr<Surfel> currIsect(nullptr);
    for(auto &prim : primitives) {
        if(prim->intersect(r, currIsect)) {
            if(isect == nullptr || currIsect->time < isect->time){
                isect = currIsect;
                isect->primitive = std::dynamic_pointer_cast<GeometricPrimitive>(prim);
            }
        }
    }
    return (isect != nullptr);
}

bool PrimList::intersect_p(const Ray& r, real_type maxT ) const {
    for(auto &prim : primitives) {
        if(prim->intersect_p(r, maxT)) return true;
    }
    return false;
}

bool GeometricPrimitive::intersect_p( const Ray& r, real_type maxT  ) const {
    return shape->intersect_p(r, maxT); 
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