#include "normal_map.h"
#include "../core/color.h"

namespace rt3{

std::optional<Color> NormalIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene) const {
    shared_ptr<Surfel> isect; // Intersection information.
    if (!scene->intersect(ray, isect)) {
        return {}; // empty object.
    }

    // normalmente, ocorre a normalização da normal
    Point3f normal = glm::normalize(isect->n);

    return Color( 
            (normal[0] + 1) * (0.5),
            (normal[1] + 1) * (0.5),
            (normal[2] + 1) * (0.5)
    );
}

NormalIntegrator* create_normal_integrator(unique_ptr<Camera> &&camera){
    return new NormalIntegrator(std::move(camera));
}

};