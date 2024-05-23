#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "rt3.h"
#include "ray.h"
#include "surfel.h"
#include "shape.h"
#include "material.h"

namespace rt3{

class Primitive {
public:
	virtual ~Primitive(){};
	virtual bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const = 0;
	virtual bool intersect_p( const Ray& r, real_type maxT ) const = 0;
};

class AggregatePrimitive : public Primitive{
public:
	vector<std::shared_ptr<Primitive>> primitives;

	AggregatePrimitive(vector<std::shared_ptr<Primitive>> &&prim) : primitives(std::move(prim)) {}

	virtual ~AggregatePrimitive(){};
};


class PrimList : public AggregatePrimitive{
public:

	PrimList(vector<std::shared_ptr<Primitive>> &&prim):AggregatePrimitive(std::move(prim)){}

	~PrimList(){};

	bool intersect_p( const Ray& r, real_type maxT ) const override;

	bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const override;

};

class GeometricPrimitive : public Primitive, public std::enable_shared_from_this<GeometricPrimitive>{
public:
	std::shared_ptr<Material> material;
	std::unique_ptr<Shape> shape;

	GeometricPrimitive(std::shared_ptr<Material> mat, std::unique_ptr<Shape> &&s) : material(mat), 
												shape(std::move(s)) {}

	~GeometricPrimitive(){};

	bool intersect_p( const Ray& r, real_type maxT  ) const override;

	bool intersect( const Ray& r, std::shared_ptr<Surfel> &isect ) const override;

	std::shared_ptr<Material> get_material() const{  return material; }
};

} // namespace rt3



#endif