#include "api.h"

#include <chrono>
#include <memory>
#include "color.h"
#include "../materials/flat.h"
#include "../integrators/ping_pong.h"
#include "../integrators/flat.h"
#include "../integrators/normal_map.h"
#include "../shapes/sphere.h"
#include "../lights/ambient.h"
#include "../lights/point.h"
#include "../lights/spot.h"
#include "../lights/directional.h"
#include "../shapes/triangle.h"

namespace rt3 {


//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
std::unique_ptr<RenderOptions> API::render_opt;
vector<std::pair<ParamSet, shared_ptr<Material>>> API::global_primitives;
vector<std::pair<std::shared_ptr<TriangleMesh>, shared_ptr<Material>>> API::global_mesh_primitives;
shared_ptr<Material> API::curr_material;
std::map<string, shared_ptr<Material>> API::named_materials;
vector<ParamSet> API::lights;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Film* API::make_film(const std::string& name, const ParamSet& ps) {
  std::cout << ">>> Inside API::make_film()\n";
  Film* film{ nullptr };
  film = create_film(ps);

  // Return the newly created film.
  return film;
}

shared_ptr<Primitive> API::make_primitive( const ParamSet& ps_accelerator, vector<shared_ptr<PrimitiveBounds>>&& primitives) {

    shared_ptr<Primitive> primitive = nullptr;

    std::string type = retrieve(ps_accelerator, "type", std::string{"list"});

    if(type == "list") {
        primitive = shared_ptr<Primitive> (new PrimList(std::move(primitives)));
    }else if(type == "bvh") {
        primitive = BVHAccel::build(std::move(primitives));
    }else {
        RT3_ERROR("Unknown accerelator type.");
    }

    return primitive;
}

Background* API::make_background(const std::string& name, const ParamSet& ps) {
  std::cout << ">>> Inside API::make_background()\n";
  Background* bkg{ nullptr };
  bkg = create_color_background(ps);

  // Return the newly created background.
  return bkg;
}

Camera* API::make_camera(const ParamSet &ps_camera, const ParamSet &ps_lookat, unique_ptr<Film> &&the_film) {
  std::cout << ">>> Inside API::make_camera()\n";
  Camera* cam{ nullptr };

  std::string type = retrieve(ps_camera, "type", std::string { "orthographic" });

  if(type == "orthographic"){
    cam = create_orthographic_camera(ps_camera, ps_lookat, std::move(the_film));
  }else if(type == "perspective"){
    cam = create_perspective_camera(ps_camera, ps_lookat, std::move(the_film));
  }

  // Return the newly created camera.
  return cam;
}

Material * API::make_material(const ParamSet &ps_material)
{
    std::cout << ">>> Inside API::make_material()\n";
    std::string type = retrieve(ps_material, "type", std::string{ "flat" });
    std::cout << ">>>> TYPE:" << type << std::endl;

    Material *material = nullptr;
    if(type == "flat"){
        material = create_flat_material(ps_material);
    } else if(type == "blinn") {
      material = create_ping_pong_material(ps_material);
    } else {
        RT3_ERROR("Uknown material type.");
    }
    std::cout << "RETURNED" << std::endl;
    // Return the newly created material
    return material;
}

Integrator * API::make_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera) {
  std::cout << ">>> Inside API::make_integrator() \n";
  std::string type = retrieve(ps_integrator, "type", std::string{ "flat" });
  Integrator *integrator = nullptr;
  if(type == "flat") {
    integrator = create_flat_integrator(std::move(camera));
  } else if(type == "normal_map") {
    integrator = create_normal_integrator(std::move(camera));
  } else if(type == "blinn_phong") {
    integrator = create_ping_pong_integrator(ps_integrator, std::move(camera));
  } else if(type == "blinn_phong") {
    integrator = create_ping_pong_integrator(ps_integrator, std::move(camera));
  } else {
    RT3_ERROR("Integrator type unknown.");
  }

  return integrator;
}

Shape * API::make_shape(const ParamSet &ps) {
  string type = retrieve(ps, "type", std::string{"sphere"});
  std::cout << ">>> Inside API::make_shape()\n";

  Shape *shape = nullptr;

  if(type == "sphere") {
    shape = create_sphere(ps);
  } else {
    RT3_ERROR("Unknown object type.");
  }

  return shape;
}

GeometricPrimitive * API::make_geometric_primitive(unique_ptr<Shape> &&shape, 
                                                   shared_ptr<Material> material) {

    //std::cout << ">>> Inside API::make_geometric_primitive()\n";

    return new GeometricPrimitive(
        material,
        std::move(shape)
    );
}

Light * API::make_light( const ParamSet &ps_light, Bounds3f world_box) {
    std::cout << ">>> Inside API::make_light()\n";
    Light* light = nullptr;

    std::string type = retrieve(ps_light, "type", std::string{"ambient"});
    if(type == "ambient") {
        light = create_ambient_light(ps_light);
    }else if(type == "point") {
        light = create_point_light(ps_light);
    } else if(type == "spot") {
        light = create_spotlight_light(ps_light);
    }else if(type == "directional") {
        light = create_directional_light(ps_light, world_box);
    }else{
        RT3_ERROR("Light type unknown.");
    }
    
    // Return the newly created integrator
    return light;
}

vector<Shape*> API::make_triangles(shared_ptr<TriangleMesh> tm) {
  std::cout << ">>> Inside API::make_triangles()\n";
  
  vector<Shape*> shapes = create_triangles(tm) ; 
  
  return shapes;
}

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ
// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine(const RunningOptions& opt) {
  // Save running option sent from the main().
  curr_run_opt = opt;
  // Check current machine state.
  if (curr_state != APIState::Uninitialized) {
    RT3_ERROR("API::init_engine() has already been called! ");
  }
  // Set proper machine state
  curr_state = APIState::SetupBlock;
  // Preprare render infrastructure for a new scene.
  render_opt = std::make_unique<RenderOptions>();
  // Create a new initial GS
  // curr_GS = GraphicsState();
  RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_up() {
  // Check for correct machine state
  if (curr_state == APIState::Uninitialized) {
    RT3_ERROR("API::clean_up() called before engine initialization.");
  } else if (curr_state == APIState::WorldBlock) {
    RT3_ERROR("API::clean_up() called inside world definition section.");
  }
  curr_state = APIState::Uninitialized;

  RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run() {
  // Try to load and parse the scene from a file.
  RT3_MESSAGE("[2] Beginning scene file parsing...\n");
  // Recall that the file name comes from the running option struct.
  parse(curr_run_opt.filename.c_str());
}

void API::world_begin() {
  VERIFY_SETUP_BLOCK("API::world_begin");  // check for correct machine state.
  curr_state = APIState::WorldBlock;       // correct machine state.
}

void API::world_end() {
  VERIFY_WORLD_BLOCK("API::world_end");
  // The scene has been properly set up and the scene has
  // already been parsed. It's time to render the scene.

  std::unique_ptr<Scene> the_scene;
  std::unique_ptr<Integrator> the_integrator;

  // MAKING THE SCENE
  std::unique_ptr<Background> the_background{ make_background(render_opt->bkg_type,
                                                              render_opt->bkg_ps) };

  vector<std::shared_ptr<PrimitiveBounds>> primitives;

  Bounds3f world_box;

  for(auto [obj_ps, mat] : global_primitives) {
    unique_ptr<Shape> shape(make_shape(obj_ps));

    world_box = Bounds3f::insert(world_box, shape->computeBounds());

    primitives.push_back(shared_ptr<PrimitiveBounds>(make_geometric_primitive(std::move(shape), mat)));
  }

  for(auto [mesh_ps, mat] : global_mesh_primitives) {
    shared_ptr<TriangleMesh> mesh_copy = mesh_ps->copy_mesh();

    vector<Shape*> shapes = make_triangles(mesh_copy);
    for(Shape* shape : shapes) {
      world_box = Bounds3f::insert(world_box, shape->computeBounds());

      primitives.push_back(shared_ptr<PrimitiveBounds>(make_geometric_primitive(std::move(unique_ptr<Shape>(shape)), mat)));
    }
  }

  //unique_ptr<PrimList> primitive = unique_ptr<PrimList>(new PrimList(std::move(primitives)));
  shared_ptr<Primitive> primitive = make_primitive(render_opt->accelerator_ps, std::move(primitives));
  
  vector<shared_ptr<Light>> the_lights;
  for (auto light_ps : lights) {
    the_lights.push_back(shared_ptr<Light>(make_light(light_ps, world_box)));
  }  
  
  the_scene = make_unique<Scene>(std::move(primitive), std::move(the_background), std::move(the_lights));
  // MADE THE SCENE

  // MAKE THE INTEGRATOR -----------------------------------------------------------------------------------
  std::unique_ptr<Film> the_film{ 
                  make_film(render_opt->film_type, 
                            render_opt->film_ps) };

  std::unique_ptr<Camera> the_camera{ 
                  make_camera(render_opt->camera_ps, 
                              render_opt->lookat_ps, 
                              std::move(the_film)) };

  the_integrator = unique_ptr<Integrator>(make_integrator(render_opt->integrator_ps, std::move(the_camera)));
  // MADE THE INTEGRATOR -----------------------------------------------------------------------------------

  // Run only if we got film and background.
  if (the_integrator and the_scene) {
    RT3_MESSAGE("    Parsing scene successfuly done!\n");
    RT3_MESSAGE("[2] Starting ray tracing progress.\n");

    // Structure biding, c++17.
    /*auto res = the_camera->film->get_resolution();
    size_t w = res[0];
    size_t h = res[1];
    RT3_MESSAGE("    Image dimensions in pixels (W x H): " + std::to_string(w) + " x "
                + std::to_string(h) + ".\n");*/
    RT3_MESSAGE("    Ray tracing is usually a slow process, please be patient: \n");

    //================================================================================
    auto start = std::chrono::steady_clock::now();

    //std::vector<real_type> cw = retrieve(render_opt->film_ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
    the_integrator->render(the_scene);  // TODO: This is the ray tracer's  main loop.

    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start;  // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds ("
                + std::to_string(std::chrono::duration<double, std::milli>(diff).count())
                + " ms) \n");
  }
  // [4] Basic clean up
  curr_state = APIState::SetupBlock;  // correct machine state.
  reset_engine();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine() {
  // curr_GS = GraphicsState();
  // This will delete all information on integrator, cameras, filters,
  // acceleration structures, etc., that has been set previously.
  //render_opt = std::make_unique<RenderOptions>();
}

void API::background(const ParamSet& ps) {
  std::cout << ">>> Inside API::background()\n";
  VERIFY_WORLD_BLOCK("API::background");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->bkg_type = type;
  // Store current background object.
  render_opt->bkg_ps = ps;
}

void API::film(const ParamSet& ps) {
  std::cout << ">>> Inside API::film()\n";
  VERIFY_SETUP_BLOCK("API::film");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->film_type = type;
  render_opt->film_ps = ps;
}

void API::camera(const ParamSet& ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::camera");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "perspective" });
  render_opt->camera_type = type;
  render_opt->camera_ps = ps;
}

void API::lookat(const ParamSet& ps) {
  std::cout << ">>> Inside API::lookat()\n";
  VERIFY_SETUP_BLOCK("API::lookat");

  render_opt->lookat_ps = ps;
}

void API::make_named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::make_named_material()\n";
  VERIFY_WORLD_BLOCK("API::make_named_material");
  VERIFY_WORLD_BLOCK("API::make_named_material");

  string material_name = retrieve(ps, "name", string());

  named_materials[material_name] = shared_ptr<Material>(make_material(ps));
}

void API::named_material(const ParamSet &ps) {
  std::cout << ">>> Inside API::named_material()\n";
  VERIFY_WORLD_BLOCK("API::named_material");

  string material_name = retrieve(ps, "name", string());

  curr_material = named_materials[material_name];
}

void API::material(const ParamSet &ps) {
  std::cout << ">>> Inside API::material()\n";
  VERIFY_WORLD_BLOCK("API::material");

  std::shared_ptr<Material> new_material(make_material(ps));

  curr_material = new_material;
}

void API::integrator(const ParamSet &ps) {
  std::cout << ">>> Inside API::integrator()\n";
  VERIFY_SETUP_BLOCK("API::integrator");

  render_opt->integrator_ps = ps;
}

void API::accelerator(const ParamSet &ps) {
  std::cout << ">>> Inside API::accelerator()\n";
  VERIFY_SETUP_BLOCK("API::accelerator");

  render_opt->accelerator_ps = ps;
}

void API::object(const ParamSet &ps) {
  std::cout << ">>> Inside API::object()\n";
  VERIFY_WORLD_BLOCK("API::object");

  std::string type = retrieve(ps, "type", string{"trianglemesh"});

  if(type == "trianglemesh") {
    if(ps.count("filename")) {
      string filename = retrieve(ps, "filename", string());

      shared_ptr<TriangleMesh> tm{new TriangleMesh()};

      bool status = load_mesh_data(
        retrieve(ps, "filename", string{}), 
        retrieve(ps, "reverse_vertex_order", false), 
        retrieve(ps, "compute_normals", false),
        retrieve(ps, "flip_normals", false), 
        tm
      );      

      if(status){
        tm->backface_cull = retrieve(ps, "backface_cull", false);
      }else{
        RT3_ERROR("Couldn't load obj file");
      }

      global_mesh_primitives.push_back({tm, curr_material});
    } else {
      global_mesh_primitives.push_back({shared_ptr<TriangleMesh>(create_triangle_mesh(ps)), curr_material});
    }
  }else{
    global_primitives.push_back({ps, curr_material});
  }  
}

void API::light(const ParamSet &ps) {
  std::cout << ">>> Inside API::light()\n";
  VERIFY_WORLD_BLOCK("API::light");

  lights.push_back(ps);
}

void API::instantiate_obj(const ParamSet &ps) {}
void API::start_obj_instance(const ParamSet &ps) {}
void API::finish_obj_instance() {}
void API::push_GS() {}
void API::pop_GS() {}
void API::push_CTM() {}
void API::pop_CTM() {}
void API::identity() {}
void API::translate(const ParamSet &ps) {}
void API::scale(const ParamSet &ps) {}      
void API::rotate(const ParamSet &ps) {}      
void API::save_coord_system(const ParamSet &ps) {}
void API::restore_coord_system(const ParamSet &ps) {}


}  // namespace rt3
