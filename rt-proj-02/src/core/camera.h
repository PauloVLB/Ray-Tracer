#ifndef CAMERA_H
#define CAMERA_H

#include "rt3.h"
#include "film.h"

namespace rt3 {
class Camera{
public:
    std::unique_ptr<Film> film;
    Point3f e; 
    Vector3f u, v, w; // the last two lines compose the frame of the camera
    ScreenWindow sw;

    Camera(unique_ptr<Film> &film, Point3f lf, Point3f la, Vector3f up);

    pair<real_type, real_type> get_uv(int i, int j);

    virtual Ray generate_ray(int x, int y) = 0;
};

class OrthographicCamera : public Camera {
    public:
        std::unique_ptr<Film> film;
        Point3f e; 
        Vector3f u, v, w; 
        ScreenWindow sw;

        OrthographicCamera(unique_ptr<Film> &film, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw): Camera(film, lf, la ,up), sw(sw) {}

        std::unique_ptr<OrthographicCamera> create_orthographic(unique_ptr<Film> &film, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw) {
            return std::make_unique<OrthographicCamera>(film, lf, la, up, sw);
        }

        Ray generate_ray(int i, int j) override {
            // auto [u2, v2] = get_uv(i, j); this could be useful instead cross and normalize ? 
            
            Vector3f w2 = normalized(lf - la);
            Vector3f u2 = normalized(cross(up, w));
            Vector3f v2 = normalized(cross(w,u)); 
            Point3f ray_origin = e + u * this->u2 + this->v2 * v;
            vector3f ray_direction = normalized(w); // This sould be normalized or not ?

            return Ray(ray_origin, ray_direction)
        }

        // Temporary functions until refactor to glm
        Vector3f normalized(const Vector3f& v) {
            float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
            if (length != 0) {
                return Vector3f(v.x / length, v.y / length, v.z / length);
            } 
        }

        Vector3f cross(const Vector3f& a, const Vector3f& b) {
            return Vector3f(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }
}

};


#endif