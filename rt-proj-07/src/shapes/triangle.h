#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../core/shape.h"
#include "triangle_mesh.h"
#include "../ext/tiny_obj_loader.h"

namespace rt3{

/// Represents a single triangle.
class Triangle : public Shape {
private:
    shared_ptr<const Point3f> vert[3];
    shared_ptr<const Normal3f> n[3];

    shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
public:
    // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
    Triangle( shared_ptr<TriangleMesh> mesh, int tri_id)
    : Shape(), mesh{mesh}
    {
        auto &v_indexes = *(mesh->vertex_indices);
        auto &n_indexes = *(mesh->normal_indices);

        auto &vertices = *(mesh->vertices);
        auto &normals = *(mesh->normals);
        // auto &uvcoords = *(mesh->uvcoords);

        // This is just a shortcut to access this triangle's data stored in the mesh database.
        for(int i = 0; i < 3; ++i){
            vert[i] = vertices[v_indexes[ 3 * tri_id + i]];
            n[i] = normals[n_indexes[ 3 * tri_id + i]];
        }

    }

    /// Return the triangle's bounding box.
    Bounds3f computeBounds() const override;
    bool tri_intersect(const Ray &r, real_type &t, real_type &u, real_type &v) const;

    bool intersect_p(const Ray &r, real_type maxT) const override;
    /// The regular intersection methods, as defined in the Shape parent class.
    bool intersect(const Ray &r, shared_ptr<Surfel> &isect) const override;

    /// This friend function helps us debug the triangles, if we want to.
    friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
};

/// This function creates the internal data structure, required by the RT3.
vector<Shape*> create_triangles(shared_ptr<TriangleMesh> mesh);

// Loads obj file at filename and then calls extract_obj_data
bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md );

// Extracts data from attrib and saves into md
// Calls retrieve functions for each step
void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, /* OUT */ shared_ptr<TriangleMesh> md );

}

#endif