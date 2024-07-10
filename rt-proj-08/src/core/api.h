#ifndef API_H
#define API_H 1

#include <string>

#include "paramset.h"
#include "rt3.h"
#include "light.h"
#include "scene.h"
#include "camera.h"
#include "integrator.h"
#include "background.h"

#include "color.h"

#include "../materials/flat.h"
#include "../materials/ping_pong.h"

#include "../integrators/flat.h"
#include "../integrators/normal_map.h"
#include "../integrators/ping_pong.h"

#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"
#include "../materials/flat.h"
#include "../materials/ping_pong.h"
#include "integrator.h"
#include "scene.h"
#include "light.h"
#include "../shapes/triangle_mesh.h"

#include "transform.h"

//=== API Macro definitions

/// Check whether the current state has been intialized.
#define VERIFY_INITIALIZED(func_name)                                          \
  if (curr_state == APIState::Uninitialized) {                                 \
    RT3_ERROR(std::string{"API::init() must be called before "} +              \
              std::string{func_name} + std::string{"(). Ignoring."});          \
  }

/// Check whether the current state corresponds to setup section.
#define VERIFY_SETUP_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::WorldBlock) {                                    \
    RT3_ERROR(std::string{"Rendering setup cannot happen inside "} +           \
              std::string{"World definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

/// Check whether the current state corresponds to the world section.
#define VERIFY_WORLD_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::SetupBlock) {                                    \
    RT3_ERROR(std::string{"Scene description must happen inside "} +           \
              std::string{"World Definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

namespace rt3 {
/// Collection of objects and diretives that control rendering, such as camera,
/// lights, prims.
struct RenderOptions {
  // the Film
  std::string film_type{"image"}; // The only type available.
  ParamSet film_ps;
  
  /// the Camera
  string camera_type{"perspective"};
  ParamSet camera_ps;
  
  /// the Look At
  ParamSet lookat_ps;
  
  /// the Bakcground
  string bkg_type{"solid"}; // "image", "interpolated"
  ParamSet bkg_ps;

  /// the Integrator
  ParamSet integrator_ps;

  /// the Accelerator
  ParamSet accelerator_ps;
};

struct ObjectBuild {
  vector<tuple<ParamSet, shared_ptr<Material>, shared_ptr<Transform>>> primitives;
  vector<tuple<shared_ptr<TriangleMesh>, shared_ptr<Material>, shared_ptr<Transform>>> mesh_primitives;

  vector<ParamSet> lights;
};

/// Static class that manages the render process
class API {
public:
  /// Defines the current state the API may be at a given time
  enum class APIState {
    Uninitialized, //!< Initial state, before parsing.
    SetupBlock,    //!< Parsing the render setup section.
    WorldBlock     //!< Parsing the world's information section.
  };

  /// Stores the running options collect in main().
  static RunningOptions curr_run_opt;
  static vector<tuple<ParamSet, shared_ptr<Material>, shared_ptr<Transform>>> global_primitives;
  static vector<tuple<shared_ptr<TriangleMesh>, shared_ptr<Material>, shared_ptr<Transform>>> global_mesh_primitives;
  static shared_ptr<Material> curr_material;
  static std::map<string, shared_ptr<Material>> named_materials;
  static std::map<string, shared_ptr<TriangleMesh>> meshes;
  static vector<ParamSet> lights;

  static Transform curr_TM;
  static std::stack<shared_ptr<Transform>> saved_TM;

  static string curr_obj;
  static ObjectBuild obj_build;
  static Dictionary<std::string, shared_ptr<ObjectBuild>> named_obj_build;

  // Static member functions
  static std::shared_ptr<const Transform> get_transform(const std::string &name);
  static void save_curr_state();
  static void restore_prev_state();

private:
  /// Current API state
  static APIState curr_state;
  /*
   * The unique pointer below is useful to support various calls to
   * init()-run()-clean-up(), in case we want to process several input files in
   * a single run of the system.
   */
  /// Unique infrastructure to render a scene (camera, integrator, etc.).
  static std::unique_ptr<RenderOptions> render_opt;
  // [NO NECESSARY IN THIS PROJECT]
  // /// The current GraphicsState
  // static GraphicsState curr_GS;
  // [NOT NECESSARY IN THIS PROJECT]
  // /// Pointer to the scene. We keep it as parte of the API because it may be
  // reused later [1] Create the integrator. static unique_ptr< Scene >
  // the_scene;

  // === Helper functions.
  ///
  static Film *make_film(const string &name, const ParamSet &ps);
  static Background *make_background(const string &name, const ParamSet &ps);
  static Camera *make_camera(const ParamSet &ps_camera, const ParamSet &ps_lookat, unique_ptr<Film> &&the_film);
  static Material *make_material( const ParamSet &ps_material);
  static Integrator *make_integrator(const ParamSet &ps_integrator, unique_ptr<Camera> &&camera);
  static Shape *make_shape(const ParamSet &ps, shared_ptr<Transform> tr);
  static GeometricPrimitive *make_geometric_primitive(unique_ptr<Shape> &&shape, shared_ptr<Material> material);
  static Light * make_light( const ParamSet &ps_light, Bounds3f worldBox);
  static vector<Shape*> make_triangles(shared_ptr<TriangleMesh> tm);
  static shared_ptr<Primitive> make_primitive( const ParamSet& ps_accelerator, vector<shared_ptr<PrimitiveBounds>>&& primitives);
public:
  //=== API function begins here.
  static void init_engine(const RunningOptions &);
  static void run();
  static void clean_up();
  static void reset_engine();

  static void film(const ParamSet &ps);
  static void camera(const ParamSet &ps);
  static void lookat(const ParamSet &ps);
  static void background(const ParamSet &ps);
  static void world_begin();
  static void world_end();
  static void material(const ParamSet &ps);
  static void make_named_material(const ParamSet &ps);
  static void named_material(const ParamSet &ps);
  static void integrator(const ParamSet &ps);
  static void object(const ParamSet &ps);
  static void light(const ParamSet &ps);
  static void accelerator(const ParamSet &ps);
  static void instantiate_obj(const ParamSet &ps);
  static void start_obj_instance(const ParamSet &ps);
  static void finish_obj_instance();
  static void push_GS();
  static void pop_GS();
  static void push_CTM();
  static void pop_CTM();
  static void identity();
  static void translate(const ParamSet &ps);
  static void scale(const ParamSet &ps);      
  static void rotate(const ParamSet &ps);      
  static void save_coord_system(const ParamSet &ps);
  static void restore_coord_system(const ParamSet &ps);   
};
} // namespace rt3

#endif // API_H
