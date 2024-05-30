#include "directional.h"

namespace rt3{

tuple<Color, Vector3f, unique_ptr<VisibilityTester>> DirectionalLight::sample_Li(const shared_ptr<Surfel>& hit){

    Point3f position = hit->p + (direction * -min_dist);

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        position, 
        Vector3f(),
        direction,
        min_dist
    );

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, Vector3f, unique_ptr<VisibilityTester>>{
        color_int,
        direction,
        unique_ptr<VisibilityTester>(visTester),
    };
}

DirectionalLight* create_directional_light( const ParamSet &ps, Bounds3f worldBox ){
    Point3f from = retrieve(ps, "from", Point3f());
    Point3f to = retrieve(ps, "to", Point3f());

    real_type world_diag = glm::length(worldBox.min_point - worldBox.max_point);

    return new DirectionalLight(
        retrieve(ps, "L", Color()),
        retrieve(ps, "scale", Vector3f()),
        glm::normalize(to - from),
        world_diag
    );
}

}