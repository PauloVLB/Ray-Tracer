#ifndef AABB_H
#define AABB_H

#include "primitive.h"

namespace rt3 {

class AABB {
public:
	aabb() {}
    aabb(const Vector3f& a, const Vector3f& b) {min = a; max = b}
    Vector3f min() const {return min};
    Vector3f max() const {return max};

    bool hit(const Ray r, float tmin, float tmax) const {
        for (int a = 0; a < 3; a++) { // Why 3 ?
            float t0 = ffmin((min[a] - r.origin[a]) / r.direction[a], (max[a] - r.origin[a] / r.direction[a]));
            float t1 = ffmax((min[a] - r.origin[a]) / r.direction[a], (max[a] - r.origin[a] / r.direction[a]));

            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin) {
                return false;
            }

            return true;
        }

    }
    Vector3f min;
    Vector3f max;
};

} // namespace rt3
#endif // AABB_H
