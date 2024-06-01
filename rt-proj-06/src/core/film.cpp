#include "film.h"
#include <cmath>

#include "api.h"
#include "image_io.h"
#include "paramset.h"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt } {
  m_color_buffer_ptr = std::make_unique<ColorBuffer>(resolution.y, resolution.x);
}

Film::~Film() = default;

/// Add the color to image.
void Film::add_sample(const Point2i& pixel_coord, const Color& pixel_color) {
  /* std::cout << "Adding Sample: " << std::endl;
  std::cout << pixel_coord.x << " " << pixel_coord.y << ": " << std::endl; */

  m_color_buffer_ptr->mat[pixel_coord.x][pixel_coord.y] = Color(pixel_color);

  /* std::cout << m_color_buffer_ptr->mat[pixel_coord.x][pixel_coord.y].r << " ";
  std::cout << m_color_buffer_ptr->mat[pixel_coord.x][pixel_coord.y].g << " ";
  std::cout << m_color_buffer_ptr->mat[pixel_coord.x][pixel_coord.y].b << std::endl; */
}

/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image() const {
  // TODO: call the proper writing function, either PPM or PNG.

  
  bool ok = false;
  /* for(int i = 0; i < width()*height()*3; i++) {
    std::cout << i << " = " << (int) bytes[i] << std::endl;
  } */

  std::cout << "Try to save file in " << m_filename << std::endl;
  
  if(m_image_type == image_type_e::PNG) {
    byte* bytes = m_color_buffer_ptr->get_byte_arr(4);
    ok = save_png(bytes, width(), height(), 4, m_filename);
  } else if(m_image_type == image_type_e::PPM3) {
    byte* bytes = m_color_buffer_ptr->get_byte_arr(3);
    ok = save_ppm3(bytes, width(), height(), 3, m_filename);
  } else if(m_image_type == image_type_e::PPM6) {
    byte* bytes = m_color_buffer_ptr->get_byte_arr(3);
    ok = save_ppm6(bytes, width(), height(), 3, m_filename);
  }

  if(!ok) RT3_ERROR("Could not save the image.");
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
Film* create_film(const ParamSet& ps) {
  std::cout << ">>> Inside create_film()\n";
  std::string filename;
  // Let us check whether user has provided an output file name via
  // command line arguments in main().
  if (not API::curr_run_opt.outfile.empty()) {
    // Retrieve filename defined via CLI.
    filename = API::curr_run_opt.outfile;
    // Retrieve another filename, now from the ParamSet object.
    std::string filename_from_file = retrieve(ps, "filename", std::string{ "" });
    if (not filename_from_file.empty()) {  // We also get a filename from scene file...
      RT3_WARNING(string{ "Output filename supplied on command line, \"" }
                  + API::curr_run_opt.outfile
                  + string{ "\" is overriding filename provided in scene "
                            "description file, \"" }
                  + filename_from_file);
    }
  } else {
    // Try yo retrieve filename from scene file.
    filename = retrieve(ps, "filename", std::string{ "image.png" });
  }

  // Read resolution.
  // Aux function that retrieves info from the ParamSet.
  int xres = retrieve(ps, "x_res", int(1280));
  // Aux function that retrieves info from the ParamSet.
  int yres = retrieve(ps, "y_res", int(720));
  // Quick render?
  if (API::curr_run_opt.quick_render) {
    // decrease resolution.
    xres = std::max(1, xres / 4);
    yres = std::max(1, yres / 4);
  }

  // TODO
  // Read crop window information.
  std::vector<real_type> cw = retrieve(ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
  for (const auto& e : cw) {
    std::cout << e << " ";
  }
  std::cout << '\n';

  std::string img_t_str = retrieve(ps, "img_type", std::string{"png"});

  Film::image_type_e img_t = Film::image_type_e::PNG;
  if(img_t_str == "ppm3") {
    img_t = Film::image_type_e::PPM3;
  } else if(img_t_str == "ppm6") {
    img_t = Film::image_type_e::PPM6;
  }
  // Note that the image type is fixed here. Must be read from ParamSet, though.
  return new Film(Point2i{ xres, yres }, filename, img_t);
}
}  // namespace rt3
