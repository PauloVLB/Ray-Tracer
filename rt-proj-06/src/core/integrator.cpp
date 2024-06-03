#include "integrator.h"
#include "material.h"

namespace rt3{

void SamplerIntegrator::render( const unique_ptr<Scene> &scene ) {
    int w = camera->film->width();
    int h = camera->film->height();

    const int max_chars = 100;
    const int max_it = h*w;
    const int step = max_it/max_chars;
    const int step_cycle = 100;
    const char simbol = '.';
    const char cycle[4] = {'\\', '|', '/', '-'};
    int it_bar = 0;
    int curr_it = 0;
    
    for(int i = 0; i < h; i++) {
        for(int j = 0; j < w; j++) {
            // Determine the ray for the current camera type.
            Point2f screen_coord{ float(j)/float(w), float(i)/float(h) };
            Ray ray = camera->generate_ray(i,j); // Generate the ray from (x,y)
            // Determine the incoming light.
            auto temp_L =  Li( ray, scene );
            Color L = (temp_L.has_value()) ?  temp_L.value() : scene->background->sampleXYZ(screen_coord) ;
            // Add color (radiance) to the image.
            camera->film->add_sample( Point2i( i, j ), L ); // Set color of pixel (x,y) to L.
            
            // Loading bar
            if(curr_it % step == 0 || curr_it % step_cycle == 0) {
                int curr_chars = (curr_it*max_chars)/max_it;
                std::cout << "\t " << cycle[it_bar] << " " << (curr_it*100/max_it) << "% [";
                for(int k = 0; k < max_chars; k++) {
                    if(k < curr_chars) {
                        std::cout << simbol;
                    } else {
                        std::cout << ' ';
                    }
                }
                std::cout << "]\r";
                std::cout.flush();
                it_bar = (it_bar + 1)%4;
            }
            curr_it++;
        }
    }

    std::cout << "\t" << "100% [";
    for(int k = 0; k < max_chars; k++) {
        std::cout << simbol;
    }
    std::cout << ']' << std::endl;

    camera->film->write_image();

    return;
}

} // namespace rt3
