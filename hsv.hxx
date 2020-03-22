#ifndef comp3400_2020w_project_hsv_hxx_
#define comp3400_2020w_project_hsv_hxx_

//===========================================================================

#include <cmath>
#include "rgb.hxx"

//===========================================================================

struct hsv
{
  using value_type = float;
  float hue, saturation, value;

  constexpr hsv() :
    hue{},
    saturation{},
    value{}
  {
  }

  constexpr hsv(hsv const&) = default;
  constexpr hsv& operator =(hsv const&) = default;

  constexpr hsv(value_type const& h, value_type const& s, value_type const& l) :
    hue{std::clamp(h, value_type{0}, value_type{360})},
    saturation{std::clamp(s, value_type{0}, value_type{1})},
    value{std::clamp(l, value_type{0}, value_type{1})}
  {
    if (hue == value_type{360})
      hue = value_type{};
  }
};

// See: https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
inline rgb hsv_to_rgb(hsv const& x)
{
  using value_type = hsv::value_type;

  // Compute the chroma...
  value_type const chroma = x.value * x.saturation;

  // determine hue / 60 degrees factor...
  auto const hue_factor = 
    static_cast<unsigned short>(trunc(x.hue / value_type(60))) % 6
  ;

  // chroma is largest colour, determine second largest...
  value_type const chroma2 = chroma * (1 - (hue_factor % 2));

  // adjust for lightness and return...
  value_type const adjust = x.value - chroma;

  switch (hue_factor)
  {
    case 0:
      return rgb{chroma+adjust, chroma2+adjust, adjust};
    case 1:
      return rgb{chroma2+adjust, chroma+adjust, adjust};
    case 2:
      return rgb{adjust, chroma+adjust, chroma2+adjust};
    case 3:
      return rgb{adjust, chroma2+adjust, chroma+adjust};
    case 4:
      return rgb{chroma2+adjust, adjust, chroma+adjust};
    case 5:
      return rgb{chroma+adjust, adjust, chroma2+adjust};
    default:
      return rgb{};
  }
}

inline ppm_pixel to_ppm_pixel(hsv const& x)
{
  return to_ppm_pixel(hsv_to_rgb(x));
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_hsv_hxx_
