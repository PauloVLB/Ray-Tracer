#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rt3.h"
#include "material.h"
#include "dictionary.h"
#include <memory>
#include <string>

namespace rt3 {

class Graphics_State {
public:
    std::shared_ptr<Material> curr_material;  
    bool flip_normals{false};                 
    using DictOfMat = Dictionary<std::string, std::shared_ptr<Material>>;
    std::shared_ptr<DictOfMat> mats_lib;      
    bool mats_lib_cloned{false};              

    Graphics_State() = default;
    ~Graphics_State() = default;
};

}

#endif // GRAPHICS_H
