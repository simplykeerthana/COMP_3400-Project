#ifndef comp3400_2020w_project_rgb_hxx_
#define comp3400_2020w_project_rgb_hxx_

//===========================================================================

#include <algorithm>      // For std::clamp
#include "ppm_pixel.hxx"

//===========================================================================

struct rgb
{
  using value_type = float;
  float red, green, blue;

  constexpr rgb() :
    red{},
    green{},
    blue{}
  {
  }

  constexpr rgb(rgb const&) = default;
  constexpr rgb& operator =(rgb const&) = default;

  constexpr rgb(value_type const& r, value_type const& g, value_type const& b) :
    red{std::clamp(r, value_type{0}, value_type{1})},
    green{std::clamp(g, value_type{0}, value_type{1})},
    blue{std::clamp(b, value_type{0}, value_type{1})}
  {
  }
};

inline ppm_pixel to_ppm_pixel(rgb const& value)
{
  return ppm_pixel(
    value.red * ppm_pixel::component_max(),
    value.green * ppm_pixel::component_max(),
    value.blue * ppm_pixel::component_max()
  );
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_rgb_hxx_
