#include "point.h"

namespace rt3{

tuple<Color, Vector3f, unique_ptr<VisibilityTester>> PointLight::sample_Li(const shared_ptr<Surfel>& hit){

    Vector3f direction = hit->p - position;

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        position,  // p
        Vector3f(),
        glm::normalize(direction),
        glm::length(direction)
    );

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, Vector3f, unique_ptr<VisibilityTester>>{
        color_int,
        glm::normalize(direction),
        unique_ptr<VisibilityTester>(visTester),
    };
}

PointLight* create_point_light( const ParamSet &ps ){
    return new PointLight(
        retrieve(ps, "I", Color()),
        retrieve(ps, "scale", Vector3f()),
        retrieve(ps, "from", Point3f())
    );
}

}