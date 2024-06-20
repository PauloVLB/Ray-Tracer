#include "triangle_mesh.h"

namespace rt3{

TriangleMesh *create_triangle_mesh(const ParamSet &ps){
    auto n = retrieve(ps, "ntriangles", 1);
    auto backface_cull = retrieve(ps, "backface_cull", false);
    
    auto indices_vec = retrieve(ps, "indices", std::vector<int>{0,0,0});
    auto indices = make_shared<vector<int>>(indices_vec);

    auto vertices_vec = retrieve(ps, "vertices", std::vector<Point3f>({{0, -0.5, -0.2}, {0.2, -0.5, -0.2}, {0.3, -0.5, -0.3}}));
    std::vector<shared_ptr<Point3f>> vec_ptr;
    for(auto x : vertices_vec) {
        vec_ptr.push_back(make_shared<Point3f>(x));
    }
    auto vertices = make_shared<vector<shared_ptr<Point3f>>>(vec_ptr);


    auto normals_vec = retrieve(ps, "normals", std::vector<Normal3f>({{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}));
    std::vector<shared_ptr<Normal3f>> norm_ptr;
    for(auto x : normals_vec) {
        norm_ptr.push_back(make_shared<Normal3f>(x));
    }
    auto normals = make_shared<vector<shared_ptr<Normal3f>>>(norm_ptr);

    if(retrieve(ps, "reverse_vertex_order", false)){
        auto it = indices->begin();
        while(it != indices->end()){
            reverse(it, it + 3);
            it += 3;
        }
    }

    if(ps.count("normals") == 0 || retrieve(ps, "compute_normals", false)){
        RT3_ERROR("Not implemented.");
    }

    return new TriangleMesh(n, backface_cull, indices, vertices, normals);
}

Normal3f compute_normals(shared_ptr<Point3f> a, shared_ptr<Point3f> b, shared_ptr<Point3f> c){
  Vector3f edges[2] = {*a - *b, *c - *a};
  return glm::cross(edges[0], edges[1]);
}

shared_ptr<TriangleMesh> TriangleMesh::copy_mesh() const{
    shared_ptr<vector<shared_ptr<Point3f>>> new_verts = make_shared<vector<shared_ptr<Point3f>>>(vector<shared_ptr<Point3f>>());
    shared_ptr<vector<shared_ptr<Normal3f>>> new_normals = make_shared<vector<shared_ptr<Normal3f>>>(vector<shared_ptr<Normal3f>>());

    for(auto &v : *vertices) new_verts->push_back(make_shared<Point3f>(*v));
    for(auto &n : *normals) new_normals->push_back(make_shared<Normal3f>(*n));

    return make_shared<TriangleMesh>(
        n_triangles,
        backface_cull,
        vertex_indices,
        normal_indices,
        new_verts,
        new_normals
    );  
}

}