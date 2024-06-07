#ifndef ORTOGRAPHIC_H
#define ORTOGRAPHIC_H

#include "../core/camera.h"

namespace rt3 {

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(std::unique_ptr<Film> &&film, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw);
    ~OrthographicCamera() = default;

    Ray generate_ray(int i, int j) override;
};

OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, std::unique_ptr<Film>&& the_film
);

}

#endif