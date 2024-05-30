#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "camera.h"
#include "surfel.h"
#include "scene.h"

namespace rt3 {

class Integrator {
public:
    virtual ~Integrator(){};
    virtual void render( const unique_ptr<Scene>& ) = 0;
};

class SamplerIntegrator : public Integrator {
//=== Public interface
public:
    virtual ~SamplerIntegrator(){};
    SamplerIntegrator( unique_ptr<Camera> &&_camera ){
        camera = std::move(_camera);
    }

    virtual std::optional<Color> Li(const Ray&, const unique_ptr<Scene>&) const = 0;
    virtual void render( const unique_ptr<Scene>& );
    // virtual void preprocess( const unique_ptr<Scene>& );
    
protected:
    std::unique_ptr<Camera> camera;
};


} // namespace rt3

#endif // INTEGRATOR_H