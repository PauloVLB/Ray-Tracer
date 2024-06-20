#include "camera.h"

namespace rt3 {

std::pair<real_type, real_type> Camera::get_uv(int i, int j) {
    real_type u = sw.width() * (j + 0.5);
    u /= film->width();
    u += sw.l;

    real_type v = sw.height() * (i + 0.5);
    v /= film->height();
    v += sw.b;
    
    return {u, v};
}
Camera::Camera(std::unique_ptr<Film> &&_film, Point3f lf, Point3f la, Vector3f up, ScreenWindow _sw) 
: film(std::move(_film)), e(lf), sw(_sw)
{
    Vector3f gaze = la - lf;
    
    w = glm::normalize(gaze);
    u = glm::normalize(glm::cross(up, w));
    v = glm::normalize(glm::cross(u, w));
    
    std::cout << "Gaze: ";
    std::cout << gaze[0] << " " << gaze[1] << " " << gaze[2] << "\n";
    
    std::cout << "Up: ";
    std::cout << up[0] << " " << up[1] << " " << up[2] << "\n";

    std::cout << "W: ";
    std::cout << w[0] << " " << w[1] << " " << w[2] << "\n";

    std::cout << "U: ";
    std::cout << u[0] << " " << u[1] << " " << u[2] << "\n";
    
    std::cout << "V: ";
    std::cout << v[0] << " " << v[1] << " " << v[2] << "\n";
    
}

}
