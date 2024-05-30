#include "perspective.h"

namespace rt3 {

PerspectiveCamera::PerspectiveCamera
    (std::unique_ptr<Film> && f, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw) :
    Camera(std::move(f), lf, la, up, sw) {}

Ray PerspectiveCamera::generate_ray(int i, int j) {
    auto [u2, v2] = get_uv(i, j);
    Vector3f dir = w + (u * u2) + (v * v2);
    return Ray{e, dir};
}

PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_lookat, 
    std::unique_ptr<Film>&& the_film
) {
    ScreenWindow sw;

    if(ps_camera.count("screen_window")) { // Try to retrieve screen window
        sw = retrieve(ps_camera, "screen_window", ScreenWindow());
    } else {
        if(ps_camera.count("fovy")) { // If it doesn't exists, infer with the fovy
            real_type fovy = Radians(retrieve(ps_camera, "fovy", real_type{}));

            std::cout << "TEM QUE PEGAR O VOVY " << fovy << std::endl; 
            // crazy math
            real_type h = fabs(tan(fovy/2.0));
            real_type asp = the_film->aspect_ratio();
            sw = ScreenWindow(-1 * h * asp, h * asp, -1 * h, h);
        } else { // If fovy doesn't exists, we have an error
            RT3_ERROR("Could not infer Screen Window.");
        }
    }
    
    return new PerspectiveCamera(
        std::move(the_film),
        retrieve( ps_lookat, "look_from",  Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_lookat, "look_at",    Point3f({0.0, 0.1, 0.0})),
        retrieve( ps_lookat, "up",         Vector3f({0.0, 0.1, 0.0})),
        sw
    );
}

}