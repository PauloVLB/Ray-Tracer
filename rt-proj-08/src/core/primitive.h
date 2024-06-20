#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "ray.h"
#include "surfel.h"
#include "shape.h"
#include "material.h"
#include "bounds.h"

namespace rt3{

class Primitive {
public:
	virtual ~Primitive(){};
	virtual bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const = 0;
	virtual bool intersect_p( const Ray& r, real_type maxT ) const = 0;
};

class PrimitiveBounds : public Primitive {
public:
	Bounds3f bound_box;
	PrimitiveBounds(Bounds3f bb):bound_box(bb){}
	virtual ~PrimitiveBounds(){}
	Bounds3f getBoundBox() { return bound_box; }
};

class AggregatePrimitive : public PrimitiveBounds {
public:
	vector<std::shared_ptr<PrimitiveBounds>> primitives;

	AggregatePrimitive(vector<std::shared_ptr<PrimitiveBounds>> &&prim) : 
		PrimitiveBounds(prim[0]->getBoundBox()), primitives(std::move(prim)) {
			for(int i = 1; i < (int) primitives.size(); i++) {
				bound_box = Bounds3f::insert(bound_box, primitives[i]->getBoundBox());
			}
		}

	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:

	PrimList(vector<std::shared_ptr<PrimitiveBounds>> &&prim) : AggregatePrimitive(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const override;

};

class GeometricPrimitive : public PrimitiveBounds, public std::enable_shared_from_this<GeometricPrimitive>{
public:
	std::shared_ptr<Material> material;
	std::unique_ptr<Shape> shape;

	GeometricPrimitive(std::shared_ptr<Material> mat, std::unique_ptr<Shape> &&s);

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r, real_type maxT  ) const override;

	bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const override;

	std::shared_ptr<Material> get_material() const{  return material; }
};

class BVHAccel : public AggregatePrimitive {
public:
    BVHAccel(vector<std::shared_ptr<PrimitiveBounds>> &&prim) : AggregatePrimitive(std::move(prim)) {}

    ~BVHAccel() {}

    bool intersect_p(const Ray& r, real_type maxT) const override;

    bool intersect(const Ray& r, std::shared_ptr<Surfel>& isect) const override;

    static std::shared_ptr<BVHAccel> build(vector<std::shared_ptr<PrimitiveBounds>> &&prim);
};


} // namespace rt3



#endif