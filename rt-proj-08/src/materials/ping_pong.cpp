#include "ping_pong.h"

namespace rt3{

PingPongMaterial * create_ping_pong_material( const ParamSet &ps ){
    std::cout << "Creating PingPongMaterial" << std::endl;
    return new PingPongMaterial(
        retrieve(ps, "ambient", Color()),
        retrieve(ps, "diffuse", Color()),
        retrieve(ps, "specular", Color()),
        retrieve(ps, "mirror", Color()),
        retrieve(ps, "glossiness", real_type(256))
    );
}

}