#include "flat.h"

namespace rt3 {
    FlatMaterial * create_flat_material( const ParamSet &ps ){
        return new FlatMaterial(
            retrieve(ps, "color", Color{0, 0, 0}) // I had to change this
        );
    }
}