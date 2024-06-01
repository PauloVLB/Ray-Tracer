#include "triangle.h"

namespace rt3 {

bool Triangle::tri_intersect(const Ray &r, real_type &t, real_type &u, real_type &v) const {
    Vector3f edge[2] = {*vert[1] - *vert[0], *vert[2] - *vert[0]};

	Vector3f h = glm::cross(r.d, edge[1]);
	
	float a = glm::dot(edge[0], h);

    //constexpr float EPS = std::numeric_limits<float>::epsilon();
    constexpr float EPS = 1e-3;

	if(abs(a) < EPS) return false; 

	real_type f = 1 / a;
	Vector3f s = r.o - *vert[0];

	u = f * glm::dot(s, h); 
	if(u < 0.0 || u > 1.0) return false; 

	Vector3f q = glm::cross(s, edge[0]);
	v = f * glm::dot(r.d, q); 
	if (v < 0.0 || u + v > 1.0) return false; 

	t = f * glm::dot(edge[1], q);
	if (t < EPS) return false;

	return true;
}

bool Triangle::intersect_p(const Ray &r, real_type maxT) const{
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

    // This is how we retrieve the information associated with this particular triangle.
    const Point3f &p0 = *vert[0]; // Get the 3D coordinate of the 0-vertex of this triangle.
    const Point3f &p1 = *vert[1]; // Same for the 1-vertex.
    const Point3f &p2 = *vert[2]; // Same for the 2-vertex.

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;
    Vector3f ray_cross_e2 = glm::cross(r.d, edge2);
    float det = glm::dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector3f s = r.o - p0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    Vector3f s_cross_e1 = glm::cross(s, edge1);
    float v = inv_det * glm::dot(r.d, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && t < r.t_max) // ray intersection
    {
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;

    return false;
}

bool Triangle::intersect(const Ray &r, shared_ptr<Surfel> &isect) const{
	// This is how we retrieve the information associated with this particular triangle.
    const Point3f &p0 = *vert[0]; // Get the 3D coordinate of the 0-vertex of this triangle.
    const Point3f &p1 = *vert[1]; // Same for the 1-vertex.
    const Point3f &p2 = *vert[2]; // Same for the 2-vertex.
    
    const Normal3f &n0 = *n[0]; // Retrieve the normal at vertex 0.
    const Normal3f &n1 = *n[1]; // Retrieve the normal at vertex 1.
    const Normal3f &n2 = *n[2]; // Retrieve the normal at vertex 2.

    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;
    Vector3f ray_cross_e2 = glm::cross(r.d, edge2);
    float det = glm::dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector3f s = r.o - p0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    Vector3f s_cross_e1 = glm::cross(s, edge1);
    float v = inv_det * glm::dot(r.d, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && t < r.t_max) // ray intersection
    {
        isect = unique_ptr<Surfel>(new Surfel(r(t), rt3::Lerp(v,rt3::Lerp(u, n0, n1),n2), glm::normalize(-r.d), t));
        return true;
    }
    // This means that there is a line intersection but not a ray intersection.
    return false;
}

Bounds3f Triangle::computeBounds() const {
	return Bounds3f::createBox({*vert[0], *vert[1], *vert[2]});
}

vector<Shape*> create_triangles(shared_ptr<TriangleMesh> mesh){
	vector<Shape*> tris;
	for (int i = 0; i < mesh->n_triangles; i++) {
		tris.push_back(new Triangle(mesh, i));
    }
	return tris;
}


bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md ) {
    // Default load parameters
    const char* basepath = NULL;
    bool triangulate = true;

    std::cout << "Loading " << filename << std::endl;

    // This is the tinyobj internal data structures.
    tinyobj::attrib_t attrib; // Hold all the data, such as vertex, normals, UV coords
    std::vector<tinyobj::shape_t> shapes; // Represents a collection of faces, grouped in a unit.
    std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

    // Warnings and Error values.
    std::string warn, err;


    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
            basepath, triangulate);
    
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!ret) {
        std::cout << "Failed to load/parse .obj.\n";
        return false;
    }

    // Let us now "convert" or "migrate" the data from tinyobjloader data structure into out mesh data.
    extract_obj_data( attrib, shapes, // TinyObjeLoader data structures (IN)
                      rvo, cn, fn,    // Mesh modifiers (IN)
                      md );           // Reference to the mesh data to fill in. (OUT)

    return true;
}

void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, /* OUT */ shared_ptr<TriangleMesh> md){
  
  // Logging 
  
  std::cout << "-- SUMMARY of the OBJ file --\n";
  std::cout << "# of vertices  : " << (attrib.vertices.size()  / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size()   / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) 
            << std::endl;
  std::cout << "# of shapes    : " << shapes.size()                 << std::endl;
  std::cout << "-----------------------------\n";
  

  // Retrieve the complete list of vertices.
  auto n_vertices{ attrib.vertices.size()/3 };
  for ( auto idx_v{0u} ; idx_v < n_vertices; idx_v++) {
    auto vertex = make_shared<Point3f>(Point3f{
        attrib.vertices[ 3 * idx_v + 0 ],
        attrib.vertices[ 3 * idx_v + 1 ],
        attrib.vertices[ 3 * idx_v + 2 ] 
    });

    md->vertices->push_back(vertex);
  }

  // Read the normals
  //retrieve_normals(attrib, cn, fn, md);
  auto n_normals{ attrib.normals.size()/3 };

  // TODO: flip normals if requested.
  real_type flip = (fn) ? -1 : 1;

  // Do we need to compute the normals? Yes only if the user requeste or there are no normals in the file.
  if (cn || n_normals == 0){
      RT3_ERROR("Not implemented.");
  }else {
    // Read normals from file. This corresponds to the entire 'for' below.
    // Traverse the normals read from the OBJ file.
    for ( auto idx_n{0u} ; idx_n < n_normals; idx_n++){
        // Store the normal.
        auto normal = make_shared<Normal3f>(glm::normalize(Normal3f{ 
            attrib.normals[ 3 * idx_n + 0 ] * flip,
            attrib.normals[ 3 * idx_n + 1 ] * flip,
            attrib.normals[ 3 * idx_n + 2 ] * flip 
        }));

        md->normals->push_back(normal);
    }
  }
  // Read the complete list of texture coordinates.
  // retrieve_textures(attrib, md);

  // Read mesh connectivity and store it as lists of indices to the real data.
  //retrieve_shapes(shapes, rvo, md);
  auto n_shapes{ shapes.size() };
  // stringstream ss;
  md->n_triangles = 0; // We must reset this here.
  // In case the OBJ file has the triangles organized in several shapes or groups, we
  // ignore this and store all triangles as a single mesh dataset.
  // This is why we need to reset the triangle count here.
  for ( auto idx_s{0u} ; idx_s < n_shapes; idx_s++)
  {
      // ss << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
      // ss << "Size of shape["<< idx_s << "].mesh.indices: "
      //       << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size()) << '\n';
      // ss << "shape["<< idx_s << "].num_faces: "
      //     <<  static_cast<unsigned long>(shapes[idx_s].mesh.num_face_vertices.size()) << '\n';

      // For each face print out the indices lists.
      size_t index_offset = 0;
      // # of triangles for this "shape" (group).
      // NOTE that we are accumulate the number of triangles coming from the shapes present in the OBJ file.
      md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
      for ( auto idx_f{0} ; idx_f < md->n_triangles; idx_f++)
      {
          // Number of vertices per face (always 3, in our case)
          size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

          // ss << "  face[" << idx_f << "].fnum = "  << static_cast<unsigned long>(fnum) << '\n';

          // TODO: Invert order of vertices if flag is on. (DONE!)
          if ( rvo == true ) {
              // ss << "Reverse order\n";
              // For each vertex in the face print the corresponding indices
              for (int v = fnum-1; v >= 0 ; v--)
              {
                  tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                  // ss << "    face[" << idx_f << "].v[" << v << "].indices = "
                  //     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                  // Add the indices to the global list of indices we need to pass on to the mesh object.
                  md->vertex_indices->push_back( idx.vertex_index );
                  md->normal_indices->push_back( idx.normal_index );
                  // md->uvcoord_indices->push_back( idx.texcoord_index );
              }
          }
          else { // Keep the original vertex order
              // For each vertex in the face get the corresponding indices
              for (size_t v = 0; v < fnum; v++)
              {
                  tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                  // ss << "    face[" << idx_f << "].v[" << v << "].indices = "
                  //     << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                  // Add the indices to the global list of indices we need to pass on to the mesh object.
                  // This goes to the mesh data structure.
                  md->vertex_indices->push_back( idx.vertex_index );
                  md->normal_indices->push_back( idx.normal_index );
                  // md->uvcoord_indices->push_back( idx.texcoord_index );
              }
          }

          // Advance over to the next triangle.
          index_offset += fnum;
      }
  }

//   cout << "This is the list of indices: \n";

//   cout << "   + Vertices [ ";
//   std::copy( md->vertex_indices->begin(), md->vertex_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
//   cout << "]\n";

//   cout << "   + Normals [ ";
//   std::copy( md->normal_indices->begin(), md->normal_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
//   cout << "]\n";

  // cout << "   + UV coords [ ";
  // std::copy( md->uvcoord_indices->begin(), md->uvcoord_indices->end(), std::ostream_iterator< int > ( std::cout, " " ) );
  // cout << "]\n";
}

}