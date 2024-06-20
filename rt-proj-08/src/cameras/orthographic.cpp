#include "orthographic.h"


namespace rt3 {

OrthographicCamera::OrthographicCamera
    (std::unique_ptr<Film> && f, Point3f lf, Point3f la, Vector3f up, ScreenWindow sw): 
    Camera(std::move(f), lf, la, up, sw) {}


Ray OrthographicCamera::generate_ray(int i, int j) {
    auto [u2,v2] = get_uv(i,j);
    Vector3f dir = w; // Expendable
    return Ray{e + (u * u2) + (v * v2), dir};
}

OrthographicCamera* create_orthographic_camera(
    const ParamSet& ps_camera,
    const ParamSet& ps_look_at, std::unique_ptr<Film>&& the_film
) {
    
    Point3f lf = retrieve( ps_look_at, "look_from",  Point3f({0.0, 0.1, 0.0}));
    Point3f la = retrieve( ps_look_at, "look_at",  Point3f({0.0, 0.1, 0.0}));
    Vector3f uu = retrieve( ps_look_at, "up",         Vector3f({0.0, 0.1, 0.0}));
    ScreenWindow sw = retrieve( ps_camera,    "screen_window",    ScreenWindow());

    std::cout << "Created Orthographici camer with: ";
    std::cout << "\nLook From: {" << lf[0] << " " << lf[1] << " " << lf[2] << "}\n";
    std::cout << "Look At: {" << la[0] << " " << la[1] << " " << la[2] << "}\n"; 
    std::cout << "Up: {" << uu[0] << " " << uu[1] << " " << uu[2] << "}\n";
    std::cout << "The Screen Window: \n";
    std::cout << "\t" << sw.l << " " << sw.r << " " << sw.b << " " << sw.t << "\n";
    return new OrthographicCamera(
        std::move(the_film),
        lf,
        la,
        uu,
        sw
    );
}

}