#ifndef DIRECTIONAL_H
#define DIRECTIONAL_H

#include "../core/light.h"

namespace rt3{

class DirectionalLight : public LightLi {
public:
    Vector3f direction;
    real_type min_dist;

    DirectionalLight(const Color &c, const Vector3f &scl, const Vector3f &lightDirection, real_type dist=10):
        LightLi(c, scl), direction(glm::normalize(lightDirection)), min_dist(dist){}
    
    tuple<Color, Vector3f, unique_ptr<VisibilityTester>> sample_Li(const shared_ptr<Surfel>& hit) override;

};

DirectionalLight* create_directional_light( const ParamSet &ps, Bounds3f worldBox );


}



#endif