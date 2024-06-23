#include "transform.h"

namespace rt3 {
    Transform::Transform(const float mat[4][4]) {
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                    mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                    mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                    mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
    }

    Transform::Transform(const Matrix4x4 &matrix)
    : m(matrix), mInv(Inverse(matrix)) {}

    Transform::Transform(const Matrix4x4 &matrix, const Matrix4x4 &matrixInv)
    : m(matrix), mInv(matrixInv) {}

    Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }

    Transform Transpose(const Transform &t) {
        return Transform(Transpose(t.m), Transpose(t.mInv));
    }

    Transform Transform::Translate(const Vector3f &delta) const {
        Matrix4x4 m(1, 0, 0, delta.x,
                    0, 1, 0, delta.y,
                    0, 0, 1, delta.z, 
                    0, 0, 0, 1);
        Matrix4x4 minv(1, 0, 0, -delta.x,
                    0, 1, 0, -delta.y,
                    0, 0, 1, -delta.z, 
                    0, 0, 0, 1);
        return Transform(m, minv);
    }

    Transform Transform::Scale(float x, float y, float z) const {
        Matrix4x4 m(x, 0, 0, 0,
                    0, y, 0, 0,
                    0, 0, z, 0,
                    0, 0, 0, 1);
        Matrix4x4 minv(1/x, 0, 0, 0,
                    0, 1/y, 0, 0,
                    0, 0, 1/z, 0,
                    0, 0, 0, 1);
        return Transform(m, minv);
    }

    Transform Transform::RotateX(float theta) const {
        float sinTheta = std::sin(Radians(theta));
        float cosTheta = std::cos(Radians(theta));
        Matrix4x4 m(1, 0, 0, 0,
                    0, cosTheta, -sinTheta, 0,
                    0, sinTheta, cosTheta, 0,
                    0, 0, 0, 1);
        return Transform(m, Transpose(m));
    }

    Transform Transform::Rotate(float theta, const Vector3f &axis) const {
        Vector3f a = Normalize(axis);
        float sinTheta = std::sin(Radians(theta));
        float cosTheta = std::cos(Radians(theta));
        Matrix4x4 m;

        // Maybe I didn't this so efficient
        // Calculate rotations for each base matrix
        m.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
        m.m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
        m.m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
        m.m[0][3] = 0;

        m.m[1][0] = a.y * a.x * (1 - cosTheta) + a.z * sinTheta;
        m.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
        m.m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
        m.m[1][3] = 0;

        m.m[2][0] = a.z * a.x * (1 - cosTheta) - a.y * sinTheta;
        m.m[2][1] = a.z * a.y * (1 - cosTheta) + a.x * sinTheta;
        m.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
        m.m[2][3] = 0;

        m.m[3][0] = 0;
        m.m[3][1] = 0;
        m.m[3][2] = 0;
        m.m[3][3] = 1;

        return Transform(m, Transpose(m));
    }

}