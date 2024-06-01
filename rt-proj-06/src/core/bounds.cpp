#include "bounds.h"

namespace rt3{

bool Bounds3f::intersect_box(const Ray &ray, std::pair<real_type, real_type> &hits) const {
    hits = {-1e18, 1e18};

    for(int i = 0; i < 3; i++) {
        real_type inv_dir;

        if(ray.d[i] == 0) inv_dir = 1e18;
        else inv_dir = 1.0 / ray.d[i];

        real_type min_t = (min_point[i] - ray.o[i]) * inv_dir;
        real_type max_t = (max_point[i] - ray.o[i]) * inv_dir;
        if(min_t > max_t) std::swap(min_t, max_t);

        hits.first = std::max(hits.first, min_t);
        hits.second = std::min(hits.second, max_t);
    }
  
    return hits.first < hits.second;
}


bool Bounds3f::intersect_p(const Ray &ray, real_type maxT) const{
    std::pair<real_type, real_type> hits;
  
    bool intersects = intersect_box(ray, hits);
  
    if(!intersects) return false;
    else {
        if(hits.first > 0) return hits.first < maxT;
        else if(hits.second > 0) return hits.second < maxT;
        else return false;
    }
}

Bounds3f Bounds3f::insert(const Bounds3f &a, const Bounds3f &b){
    Bounds3f x{a};
    for(int i = 0; i < 3; ++i){
        x.min_point[i] = std::min(a.min_point[i], b.min_point[i]);
        x.max_point[i] = std::max(a.max_point[i], b.max_point[i]);
    }
    return x;
}

}