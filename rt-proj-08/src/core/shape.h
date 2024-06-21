#ifndef SHAPE_H
#define SHAPE_H

#include "paramset.h"
#include "surfel.h"
#include "bounds.h"

namespace rt3{

class Shape{
public:
    bool flip_normals;

    virtual ~Shape(){}

    virtual Bounds3f computeBounds() const = 0;

    virtual bool intersect_p(const Ray &r, real_type maxT ) const = 0;
    virtual bool intersect(const Ray &r, std::shared_ptr<Surfel> &isect) const = 0;

    // This really should be here?
    const Transform * obj_to_world; //!< Object to World transformation (scene specified).
    const Transform * world_to_obj; //!< World to Object trasformation (deduced base on O2W).
    const bool flip_normals;
};

} // namespace rt3










#endif