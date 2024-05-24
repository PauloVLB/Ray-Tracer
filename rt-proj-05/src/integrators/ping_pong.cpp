#include "ping_pong.h"

#include "../lights/ambient.h"
#include "../materials/ping_pong.h"
#include "../core/light.h"

namespace rt3{

Vector3f calc_h(const Vector3f &viewDir, const Vector3f &lightDir){
    return -glm::normalize(viewDir + lightDir);
}

std::optional<Color> PingPongIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene) const{
    Color L{0,0,0};

    auto temp_L = Li(ray, scene, 1);

    if(temp_L.has_value()) {
        return temp_L.value();
    }

    return {};
}

std::optional<Color> PingPongIntegrator::Li(const Ray& ray, const unique_ptr<Scene>& scene, int currRecurStep) const{
    shared_ptr<Surfel> isect; // Intersection information.  
    if (!scene->intersect(ray, isect)) {
        return {};
    } else {
        if(glm::dot(isect->wo, isect->n) < 0) return {};

        shared_ptr<PingPongMaterial> material = std::dynamic_pointer_cast<PingPongMaterial>(isect->primitive->get_material());
        
        Color color;
        for(auto &light : scene->lights){
            if(typeid(*light) == typeid(AmbientLight)){
                color = color + (light->color_int * material->ambient);
            }else{
                shared_ptr<LightLi> lightLi = std::dynamic_pointer_cast<LightLi>(light);

                auto [lightColor, lightDir, visTester] = lightLi->sample_Li(isect);

                if(visTester->unoccluded(scene)){ 
                    {
                        real_type coef = std::max(real_type(0), glm::dot(isect->n, -lightDir));
                        Color diffuseContrib = material->diffuse * lightColor * coef;
                        
                        color = color + diffuseContrib;
                    }
                    
                    if(material->glossiness){
                        auto h = calc_h(ray.d, lightDir);

                        real_type coef = std::max(real_type(0), glm::dot(isect->n, h));
                        coef = pow(coef, material->glossiness);
                        Color specularContrib = material->specular * lightColor * coef;

                        color = color + specularContrib;
                    }
                }
            }
        }

        Vector3f new_dir = glm::normalize((ray.d) - 2 * (glm::dot(ray.d, isect->n))*isect->n);
        Ray refl_ray = Ray(isect->p /* + new_dir * ERR */, new_dir, 0.1);

        if(currRecurStep < maxRecursionSteps){
            auto temp_L = Li(refl_ray, scene, currRecurStep + 1);

            if(temp_L.has_value()) {
                color = color + material->mirror * temp_L.value();
            }
        }


        return color;
    }
}


PingPongIntegrator* create_ping_pong_integrator(const ParamSet & ps_integrator, unique_ptr<Camera> &&camera){
    return new PingPongIntegrator(
        std::move(camera),
        retrieve(ps_integrator, "depth", int(1))
    );
}


}