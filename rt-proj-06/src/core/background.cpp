#include "background.h"

namespace rt3 {

/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Color BackgroundColor::sampleXYZ(const Point2f& pixel_ndc) const {
  Color xb = Color::lerp_color(corners[bl], corners[br], pixel_ndc.x);
  Color xt = Color::lerp_color(corners[tl], corners[tr], pixel_ndc.x);
  
  return Color::lerp_color(xt, xb, pixel_ndc.y);
}

BackgroundColor* create_color_background(const ParamSet& ps) {

  if(ps.count("color")) {
    Color c = retrieve(ps, "color", Color(0, 0, 0));
    return new BackgroundColor({c, c, c, c});
  } else {
    Color bl = retrieve( ps, "bl", Color(0, 0, 0));
    Color tl = retrieve( ps, "tl", Color(0, 0, 0));
    Color tr = retrieve( ps, "tr", Color(0, 0, 0));
    Color br = retrieve( ps, "br", Color(0, 0, 0));
    return new BackgroundColor({bl, tl, tr, br});
  }

}

}  // namespace rt3
