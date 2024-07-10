#include "transform.h"

namespace rt3{

Transform Transform::inverse() {
    return Transform(mInv, m);
}

Transform Transform::transpose() {
    return Transform(glm::transpose(m), glm::transpose(mInv));
}

bool Transform::operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
}

bool Transform::operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
}

bool Transform::IsIdentity() const { return m == Matrix4x4(1.0);}

const Matrix4x4 &Transform::GetMatrix() const { return m; }

const Matrix4x4 &Transform::GetInverseMatrix() const { return mInv; }

Point3f Transform::apply_p(const Point3f &p) const{
  Point3f ret = {
    m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
    m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
    m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3],
  };
  return ret;
}

Vector3f Transform::apply_v(const Vector3f &v) const{
  Vector3f ret = {
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z,
  };
  return ret;
}

Normal3f Transform::apply_n(const Normal3f &n) const{
  Normal3f ret = {
    mInv[0][0] * n.x + mInv[1][0] * n.y + mInv[2][0] * n.z,
    mInv[0][1] * n.x + mInv[1][1] * n.y + mInv[2][1] * n.z,
    mInv[0][2] * n.x + mInv[1][2] * n.y + mInv[2][2] * n.z,
  };
  return ret;
}

Ray Transform::apply_r(const Ray &r) const{
  return Ray(apply_p(r.o), apply_v(r.d));
}

Bounds3f Transform::apply_b(const Bounds3f &b) const{
  vector<Point3f> allPoints = b.getPoints();
  for(auto &p : allPoints) p = apply_p(p);
  return Bounds3f::createBox(allPoints);
}

Transform Transform::update(const Transform &t2) const{
  return Transform(m * t2.m, t2.mInv * mInv);
}

Transform Transform::getTranslationMatrix(const Vector3f &v){
  Matrix4x4 m = {
    {1, 0, 0, v[0]},
    {0, 1, 0, v[1]},
    {0, 0, 1, v[2]},
    {0, 0, 0, 1}
  };  
  Matrix4x4 inv_m = {
    {1, 0, 0, -v[0]},
    {0, 1, 0, -v[1]},
    {0, 0, 1, -v[2]},
    {0, 0, 0, 1}
  };
  return Transform(m, inv_m);
}

Transform Transform::getRotationMatrix(Vector3f v, const real_type degrees){
  v = glm::normalize(v);
  real_type rotCos = cos(Radians(degrees));
  real_type rotSin = sin(Radians(degrees));

  real_type xx = v.x * v.x;
  real_type xy = v.x * v.y;
  real_type xz = v.x * v.z;
  real_type yy = v.y * v.y;
  real_type yz = v.y * v.z;
  real_type zz = v.z * v.z;
  real_type compC = 1 - rotCos;
  real_type xs = v.x * rotSin;
  real_type ys = v.y * rotSin;
  real_type zs = v.z * rotSin;

  Matrix4x4 m = {
    { xx + (1 - xx) * rotCos, xy * compC - zs, xz * compC + ys, 0},
    { xy * compC + zs, yy + (1 - yy) * rotCos, yz * compC - xs, 0},
    { xz * compC - ys, yz * compC + xs, zz + (1 - zz) * rotCos, 0},
    { 0, 0, 0, 1},
  };

  return Transform(m);
}

Transform Transform::getScalingMatrix(const Point3f &p){
  Matrix4x4 m = {
    {p[0], 0, 0, 0},
    {0, p[1], 0, 0},
    {0, 0, p[2], 0},
    {0, 0, 0, 1}
  };
  Matrix4x4 inv_m = {
    {1/p[0], 0, 0, 0},
    {0, 1/p[1], 0, 0},
    {0, 0, 1/p[2], 0},
    {0, 0, 0, 1}
  };
  
  return Transform(m, inv_m);
}


Transform Transform::getIdentityMatrix(){
  return Transform();
}

}