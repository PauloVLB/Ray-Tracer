#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace rt3{

class Transform {
public:
    Transform(const float m[4][4]) {};
    ~Transform() = default;

    friend Transform Inverse(const Transform &t) {};
    friend Transform Transpose(const Transform &t) {};

    Transform Translate(const Vector3f &delta){};
    Transform Scale(Float x, Float y, Float z) {};
    Transform RotateX(Float theta) {};
    Transform Rotate(Float theta, const Vector3f &axis) {};
};

} // namespace rt3










#endif