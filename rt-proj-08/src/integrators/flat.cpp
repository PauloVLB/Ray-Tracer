#include "flat.h"
#include "../materials/flat.h"

namespace rt3 {
    std::optional<Color> FlatIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene) const {
        Color L(0,0,0); // The radiance
        // Find closest ray intersection or return background radiance.
        shared_ptr<Surfel> isect; // Intersection information.
        if (!scene->intersect(ray, isect)) {
            return {}; // empty object.
        }
        // Some form of determining the incoming radiance at the ray's origin.
        // Polymorphism in action.
        shared_ptr<FlatMaterial> fm = std::dynamic_pointer_cast<FlatMaterial>( isect->primitive->get_material() );
        // Assign diffuse color to L.
        return fm->get_color();
    }

    FlatIntegrator* create_flat_integrator(unique_ptr<Camera> &&camera){
        return new FlatIntegrator(std::move(camera));
    }
}