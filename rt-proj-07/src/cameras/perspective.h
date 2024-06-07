#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include "../core/camera.h"

namespace rt3 {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(std::unique_ptr<Film> &&film, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw);
    ~PerspectiveCamera() = default;

    Ray generate_ray(int i, int j) override;
};

PerspectiveCamera* create_perspective_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, std::unique_ptr<Film>&& the_film
);

}

#endif