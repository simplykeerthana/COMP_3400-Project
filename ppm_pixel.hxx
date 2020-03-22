#ifndef comp3400_2020w_project_ppm_pixel_hxx_
#define comp3400_2020w_project_ppm_pixel_hxx_

//===========================================================================

#include <cstdint>      // for std::uint8_t
#include <limits>       // for std::numeric_limits
#include <istream>      // for std::istream
#include <ostream>      // for std::ostream

//===========================================================================

//
// A description (and links to more information) of the PPM and other
// graphics file formats can be found here:
//
//  * https://en.wikipedia.org/wiki/Netpbm#File_formats
//
// The relevant file types for this project are the P3 and P6 file formats.
//

//===========================================================================

struct ppm_pixel
{
  using component_type = std::uint8_t;
  component_type r, g, b;

  static constexpr auto component_max() 
  { 
    return std::numeric_limits<component_type>::max();
  }

  ppm_pixel() : r(0), g(0), b(0) { }
  ppm_pixel(ppm_pixel const&) = default;
  ppm_pixel& operator =(ppm_pixel const&) = default;

  ppm_pixel(
    component_type const& r_,
    component_type const& g_,
    component_type const& b_
  ) :
    r(r_), g(g_), b(b_)
  {
  }

  ppm_pixel(component_type const& gray) :
    r(gray), g(gray), b(gray)
  {
  }

  // include project member written ppm_pixel::save() code here...
  #include "ppm_pixel_save.hxx"
};
  
inline bool operator ==(ppm_pixel const& a, ppm_pixel const& b)
{
  return a.r == b.r && a.g == b.g && a.b == b.b;
}

inline bool operator !=(ppm_pixel const& a, ppm_pixel const& b)
{
  return !(a == b);
}

inline std::istream& operator >>(std::istream& is, ppm_pixel& p)
{
  unsigned short r, g, b;
  if (is >> r >> g >> b)
  {
    p.r = r;
    p.g = g;
    p.b = b;
  }
  return is;
}

inline std::ostream& operator <<(std::ostream& os, ppm_pixel const& p)
{
  return p.save(os);
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_ppm_pixel_hxx_
