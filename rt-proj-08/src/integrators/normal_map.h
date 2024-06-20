#ifndef NORMAL_MAP_H
#define NORMAL_MAP_H

#include "../core/integrator.h"

namespace rt3{

class NormalIntegrator : public SamplerIntegrator {
public:
    ~NormalIntegrator(){};

    NormalIntegrator( unique_ptr<Camera> &&_camera ):
        SamplerIntegrator(std::move(_camera)){}

    std::optional<Color> Li(const Ray&, const unique_ptr<Scene>&) const override;
};

NormalIntegrator* create_normal_integrator(unique_ptr<Camera> &&);

}

#endif