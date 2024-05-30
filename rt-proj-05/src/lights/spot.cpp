#include "spot.h"

namespace rt3{

tuple<Color, Vector3f, unique_ptr<VisibilityTester>> SpotlightLight::sample_Li(const shared_ptr<Surfel>& hit){

    Vector3f direction = hit->p - position;
    real_type angleCos = glm::dot(lightDirection, glm::normalize(direction));
    real_type angle = Degrees(acos(angleCos));

    shared_ptr<Surfel> lightSurfel = make_shared<Surfel>(
        position,  // p
        Vector3f(),
        glm::normalize(direction),
        glm::length(direction)
    );

    
    Color finalColor;
    if(angle > cutoff){
        finalColor = {0,0,0};
    }else if(angle > falloff){
        finalColor = color_int * pow(1 - ((angle - falloff) / angleInterval), 4);
    }else{
        finalColor = color_int;
    }

    VisibilityTester *visTester = new VisibilityTester(hit, lightSurfel);

    return tuple<Color, Vector3f, unique_ptr<VisibilityTester>>{
        finalColor,
        glm::normalize(direction),
        unique_ptr<VisibilityTester>(visTester),
    };
}

SpotlightLight* create_spotlight_light( const ParamSet &ps ){
    Point3f from = retrieve(ps, "from", Point3f());
    Point3f to = retrieve(ps, "to", Point3f());

    return new SpotlightLight(
        retrieve(ps, "I", Color()),
        retrieve(ps, "scale", Vector3f()),
        from,
        glm::normalize(to - from),
        retrieve(ps, "cutoff", real_type()),
        retrieve(ps, "falloff", real_type())
    );
}

}