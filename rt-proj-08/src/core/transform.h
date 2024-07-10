#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "bounds.h"

namespace rt3{

class Transform {
public:
  Matrix4x4 m, mInv;

  Transform():m(Matrix4x4(1.0)), mInv(m){}

  Transform(const Matrix4x4 &matrix) : m(matrix), mInv(glm::inverse(matrix)) { }

  Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) 
    : m(m), mInv(mInv) {}

  Transform inverse();
  Transform transpose();

  bool operator==(const Transform &t) const;
  bool operator!=(const Transform &t) const;
  bool IsIdentity() const;

  const Matrix4x4 &GetMatrix() const;
  const Matrix4x4 &GetInverseMatrix() const;

  Point3f apply_p(const Point3f &p) const;
  Vector3f apply_v(const Vector3f &v) const;
  Normal3f apply_n(const Normal3f &n) const;

  Ray apply_r(const Ray &r) const;
  Bounds3f apply_b(const Bounds3f &b) const;  
  
  Transform update(const Transform &t2) const;

  static Transform getTranslationMatrix(const Vector3f &v);
  static Transform getRotationMatrix(const Vector3f v, const real_type degrees);
  static Transform getScalingMatrix(const Point3f &p);
  static Transform getIdentityMatrix();
};


}

#endif