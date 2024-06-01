#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include "../core/rt3-base.h"

namespace rt3{

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
    int n_triangles; //!< # of triangles in the mesh.
    bool backface_cull;

    // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
    std::shared_ptr<vector<int>> vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
    std::shared_ptr<vector<int>> normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
    std::shared_ptr<vector<int>> uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

    std::shared_ptr<vector<std::shared_ptr<Point3f>>> vertices;  //!< The 3D geometric coordinates
    std::shared_ptr<vector<std::shared_ptr<Normal3f>>> normals;  //!< The 3D normals.

    // Regular constructor
    TriangleMesh()
    :vertex_indices(new vector<int>), normal_indices(new vector<int>),
    vertices(new vector<std::shared_ptr<Point3f>>()), normals(new vector<std::shared_ptr<Normal3f>>())
    {/*empty*/};

    TriangleMesh(
    int n, bool bface,
    std::shared_ptr<vector<int>> indexes,
    std::shared_ptr<vector<std::shared_ptr<Point3f>>> vertexes,
    std::shared_ptr<vector<std::shared_ptr<Normal3f>>> normal
    ):n_triangles(n), backface_cull(bface), vertex_indices(indexes), normal_indices(indexes), 
    vertices(vertexes), normals(normal){}

    TriangleMesh(
    int n, bool bface,
    std::shared_ptr<vector<int>> vertex_indexes,
    std::shared_ptr<vector<int>> normal_indexes,
    std::shared_ptr<vector<std::shared_ptr<Point3f>>> vertexes,
    std::shared_ptr<vector<std::shared_ptr<Normal3f>>> normal
    ):n_triangles(n), backface_cull(bface), vertex_indices(vertex_indexes), normal_indices(normal_indexes), 
    vertices(vertexes), normals(normal){}

    std::shared_ptr<TriangleMesh> copy_mesh() const;

    TriangleMesh( const TriangleMesh& ) = delete;
    /// Assign operator disabled.
    TriangleMesh& operator=( const TriangleMesh & ) = delete;
    /// Move constructor.
    TriangleMesh( TriangleMesh && other ) = delete;
};

TriangleMesh *create_triangle_mesh(const ParamSet &ps);

Normal3f compute_normals();
}
#endif