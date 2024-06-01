#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "film.h"
#include "ray.h"

namespace rt3 {
class Camera{
public:
    std::unique_ptr<Film> film;
    Point3f e; 
    Vector3f u, v, w; // the last two lines compose the frame of the camera
    ScreenWindow sw;

    Camera(std::unique_ptr<Film> &&film, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw);

    std::pair<real_type, real_type> get_uv(int i, int j);

    virtual Ray generate_ray(int x, int y) = 0;
};

}


#endif