#ifndef PING_PONG_INT_H
#define PING_PONG_INT_H

#include "../core/integrator.h"

namespace rt3{

class PingPongIntegrator : public SamplerIntegrator {
private:
    const int maxRecursionSteps;
public:
    ~PingPongIntegrator(){};
    PingPongIntegrator( unique_ptr<Camera> &&_camera, int depth ):
        SamplerIntegrator(std::move(_camera)), maxRecursionSteps(depth){}

    std::optional<Color> Li(const Ray&, const unique_ptr<Scene>&) const override;
    std::optional<Color> Li(const Ray&, const unique_ptr<Scene>&, int currRecurStep) const;
};


PingPongIntegrator* create_ping_pong_integrator(const ParamSet &, unique_ptr<Camera> &&);


};


#endif 