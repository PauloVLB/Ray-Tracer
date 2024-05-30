#ifndef POINT_H
#define POINT_H

#include "../core/light.h"

namespace rt3{

class PointLight : public LightLi{
public:
    Point3f position;

    PointLight(const Color &c, const Vector3f &scl, const Point3f &pos):
        LightLi(c, scl), position(pos){}

    
    tuple<Color, Vector3f, unique_ptr<VisibilityTester>> sample_Li(const shared_ptr<Surfel>& hit) override;

};

PointLight* create_point_light( const ParamSet &ps );


}



#endif