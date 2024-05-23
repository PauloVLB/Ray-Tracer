#ifndef LIGHT_H
#define LIGHT_H

#include "ray.h"
#include "scene.h"
#include "surfel.h"

namespace rt3{
// Verifica se há oclusão entre dois pontos de contato.
class VisibilityTester {
public:
    std::shared_ptr<Surfel> object_surfel, light_surfel;
    VisibilityTester() = default;

    VisibilityTester(const std::shared_ptr<Surfel>& obj, const std::shared_ptr<Surfel>& light):
        object_surfel(obj), light_surfel(light){}

    bool unoccluded(const std::unique_ptr<Scene>& scene);
};

class Light {  
public:
    Color color_int;
    Vector3f scale;
    virtual ~Light(){};

    Light(const Color &c, const Vector3f &scl) : color_int(c), scale(scl) {};
    virtual void preprocess( const Scene & ) = 0;
};

class LightLi : public Light {
public:
    LightLi(const Color &c, const Vector3f &scl):Light(c, scl){}
    virtual ~LightLi(){};
    /// Retorna a intensidade da luz, direção e o teste oclusão.
    virtual tuple<Color, Vector3f, unique_ptr<VisibilityTester>> sample_Li(const shared_ptr<Surfel>& hit) = 0;
    virtual void preprocess( const Scene & ) {};
};

}

#endif