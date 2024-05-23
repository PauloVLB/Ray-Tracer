#ifndef PING_PONG_MAT_h
#define PING_PONG_MAT_h

#include "../core/material.h"

namespace rt3{

class PingPongMaterial : public Material{
public:
    const Color ambient, diffuse, specular, mirror;
    const int glossiness;

    PingPongMaterial(
        const Color &amb,
        const Color &diffus,
        const Color &spec,
        const Color &mirro,
        int gloss
    ) : ambient(amb), diffuse(diffus), specular(spec), mirror(mirro), glossiness(gloss) {}
};


PingPongMaterial * create_ping_pong_material( const ParamSet &ps );

}

#endif