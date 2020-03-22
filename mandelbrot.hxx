#ifndef comp3400_2020w_project_mandelbrot_hxx_
#define comp3400_2020w_project_mandelbrot_hxx_

//===========================================================================

#include <cstddef>                  // for std::size_t
#include <cmath>                    // for std::abs
#include <complex>                  // for std::complex
#include <string>                   // for std::string
#include <chrono>                   // for timing and clocks
#include <fstream>                  // for std::ofstream
#include <iostream>                 // for std::cerr and std::cout

#include "ppm.hxx"
#include "hsv.hxx"
#include "utils.hxx"
#include "program_cli_args.hxx"
#include "write_par.hxx"
#include "write_image.hxx"

//===========================================================================

// See: https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
// See: https://en.wikipedia.org/wiki/Mandelbrot_set#Basic_properties
template <typename T>
std::size_t compute_mandelbrot_at(
  std::complex<T> const& c, 
  std::size_t const& max_iter
)
{
  std::complex<T> z; // zero
  std::size_t i{};
  for (; abs(z) < T(2) && i != max_iter; ++i)
    z = z*z + c;
  return i;
}

//===========================================================================

inline ppm_pixel mandelbrot_result_to_ppm_pixel(
  std::size_t const& iter, 
  std::size_t const& max_iter
)
{
#if 0
  // This will produce a grayscale colouring...
  auto const gray = (log(float(iter)) / log(max_iter)) * 256.F;
  return { static_cast<unsigned char>(gray >= 256.F ? 255.F : gray) }; 
#else
  // This will produce an old-style palette style of colouring...
  constexpr size_t num_hues = 7;
  auto const hue = float(iter % num_hues)/(num_hues-1) * 360.F;
  auto const sat = 0.8F;
  auto const val = (iter != max_iter) ? float(max_iter - iter) / max_iter : 0.F;
  return to_ppm_pixel(hsv{ hue, sat, val });
#endif
}

//===========================================================================

template <typename Real>
inline Real image_index_to_real(
  size_t const& image_idx,
  size_t const& image_dim_len,
  Real const& real_dim_len,
  Real const& real_low
)
{
  return Real(image_idx) / (image_dim_len-1) * real_dim_len + real_low;
}

//===========================================================================

template <typename Image, typename Real, typename MandelToColourFunc>
void draw_mandelbrot_set(
  Image& image,
  std::complex<Real> const& min_point,
  std::complex<Real> const& max_point,
  MandelToColourFunc&& m2c,
  std::size_t const& max_iter
)
{
  using namespace std;

  auto const image_width = image.width();
  auto const image_height = image.height();
  auto const point_width = max_point - min_point;

  // time the computation of all cells...
  auto t1 = chrono::high_resolution_clock::now();

  for (size_t i{}; i != image_width; ++i)
  {
    Real const x = 
      image_index_to_real(i, image_width,
        point_width.real(), min_point.real());
    ;
    for (size_t j{}; j != image_height; ++j)
    {
      Real const y = 
        image_index_to_real(j, image_height,
          point_width.imag(), min_point.imag());
      ;
      complex<Real> const c(x,y);
      image(i,j) = m2c(compute_mandelbrot_at(c,max_iter),max_iter);
    }
  }

  auto t2 = chrono::high_resolution_clock::now();
  chrono::duration<float> secs = t2 - t1;
  cout << "MANDELBROT_TIME: " << image_width << " by " << image_height 
    << " Mandelbrot set took " << secs.count() << " seconds.\n";
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_mandelbrot_hxx_
