#include "light.h"

namespace rt3{
    bool VisibilityTester::unoccluded(const std::unique_ptr<Scene>& scene){
        Ray light_ray = Ray(light_surfel->p, light_surfel->wo);
        return !scene->intersect_p(light_ray, light_surfel->time - ERR);
    }
}