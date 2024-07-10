#ifndef BOUNDS_H
#define BOUNDS_H

#include "rt3.h"
#include "ray.h"

namespace rt3 {

class Bounds3f {

public:
    Point3f min_point, max_point; 
    
    Bounds3f(): 
        min_point{1e18, 1e18, 1e18}, 
        max_point{-1e18, -1e18, -1e18} {}
    
    Bounds3f(Point3f min_p, Point3f max_p):
        min_point(min_p), 
        max_point(max_p){}

    bool intersect_box(const Ray &ray, std::pair<real_type, real_type> &hits) const;
    bool intersect_p(const Ray &ray, real_type maxT) const;

    static Bounds3f insert(const Bounds3f &a, const Bounds3f &b);
    static Bounds3f createBox(const vector<Point3f> &p);

    vector<Point3f> getPoints() const;
};

}

#endif