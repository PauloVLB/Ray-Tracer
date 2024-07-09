#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "rt3.h"
#include "ray.h"
#include "vector3f.h"
#include <memory>

namespace rt3{

class Transform {
public:
    Transform();
    Transform(const float m[4][4]);
    Transform(const  Matrix4x4 &m);
    Transform(const  Matrix4x4 &m, const Matrix4x4 &mInv);
    ~Transform() = default;

    friend Transform Inverse(const Transform &t);
    friend Transform Transpose(const Transform &t);

    Transform Translate(const Vector3f &delta) const;
    Transform Scale(float x, float y, float z) const;
    Transform RotateX(float theta);
    Transform Rotate(float theta, const Vector3f &axis);

    //Idk if this really will be useful, but put it here, just in case...
    // defined in pbr-book
    // bool HasScale() const;
    // bool IsIdentity() const;
    // bool operator==(const Transform &t) const;
    // bool operator!=(const Transform &t) const;

private:
    Matrix4x4 m, mInv;

};

} // namespace rt3



#endif