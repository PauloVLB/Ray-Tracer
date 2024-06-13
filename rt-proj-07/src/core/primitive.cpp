#include "primitive.h"

namespace rt3 {

bool PrimList::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    shared_ptr<Surfel> currIsect(nullptr);
    for(auto &prim : primitives) {
        if(prim->intersect(r, currIsect)) {
            if(isect == nullptr || currIsect->time < isect->time){
                isect = currIsect;
                isect->primitive = std::dynamic_pointer_cast<GeometricPrimitive>(prim);
            }
        }
    }
    return (isect != nullptr);
}

bool PrimList::intersect_p(const Ray& r, real_type maxT ) const {
    for(auto &prim : primitives) {
        if(prim->intersect_p(r, maxT)) return true;
    }
    return false;
}

GeometricPrimitive::GeometricPrimitive(std::shared_ptr<Material> mat, std::unique_ptr<Shape> &&s) :
	 	PrimitiveBounds(s->computeBounds()),
		material(mat), 
		shape(std::move(s)) {}

bool GeometricPrimitive::intersect_p( const Ray& r, real_type maxT  ) const {
    return shape->intersect_p(r, maxT); 
}

bool GeometricPrimitive::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    /* std::cout << "oi" << std::endl;
    return shape->intersect(r, isect); */
    if(shape->intersect(r, isect)){
        auto shared_this = shared_ptr<const GeometricPrimitive>(shared_from_this());
        isect->primitive = shared_this;
        return true;
    } else {
        return false; 
    }
}

bool cmp(shared_ptr<PrimitiveBounds> a, shared_ptr<PrimitiveBounds> b) {
    return a->getBoundBox().min_point.x < b->getBoundBox().min_point.x;
}

bool BVHAccel::intersect_p( const Ray& r, real_type maxT ) const {
    if(bound_box.intersect_p(r, maxT)) {
        for(auto &prim : primitives) {
            if(prim->intersect_p(r, maxT)) return true;
        }
        return false;
    }else return false;
}

bool BVHAccel::intersect(const Ray &r, shared_ptr<Surfel> &isect ) const {
    pair<real_type, real_type> t;
    if(!bound_box.intersect_box(r, t)) return false;

    shared_ptr<Surfel> currIsect(nullptr);

    for(auto &prim : primitives) {   
        if(prim->intersect(r, currIsect)) {
            if(isect == nullptr || currIsect->time < isect->time) {
                isect = currIsect;
            }
        }
    }

    return (isect != nullptr);
}

std::shared_ptr<BVHAccel> BVHAccel::build(vector<std::shared_ptr<PrimitiveBounds>> &&prim) {
    vector<shared_ptr<PrimitiveBounds>> primitives{std::move(prim)};

    sort(primitives.begin(), primitives.end(), cmp);

    vector<shared_ptr<BVHAccel>> tree;
    for(int i = 0; i < (int) primitives.size(); i++){
        vector<shared_ptr<PrimitiveBounds>> leaf{primitives[i]};

        //leaf.push_back(primitives[i]);
        
        shared_ptr<BVHAccel> node{ new BVHAccel(std::move(leaf)) };
        tree.push_back(node);
    }
    
    while(tree.size() != 1) {
        vector<shared_ptr<BVHAccel>> new_tree;
        for(int i = 0; i < (int) tree.size(); i += 2){

            shared_ptr<BVHAccel> new_node = nullptr;

            if(i + 1 == (int) tree.size()){
                new_node = shared_ptr<BVHAccel>(new BVHAccel({tree[i]}));
            }else{
                new_node = shared_ptr<BVHAccel>(new BVHAccel({tree[i], tree[i+1]}));
            }

            new_tree.push_back(new_node);
        }
        tree = std::move(new_tree);
    }

    return tree[0];
}


}