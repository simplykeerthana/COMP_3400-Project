//===========================================================================
//Noah Campbell (10453611)
//Keerthana Madhavan (104995097)
//Spencer Briguglio (103746720)
//Jykee John Pavo (104891924)
//Taqiuddin Farooqui (104073892)

#include <concepts>           // new in C++20

#include "ppm_pixel.hxx"
#include "ppm.hxx"
#include "rgb.hxx"
#include "hsv.hxx"
#include "utils.hxx"
#include "mandelbrot.hxx"
#include "mandelbrot_par.hxx"

//===========================================================================
// Your code is expected to pass all of the concepts defined in this file
// (which are static_assert()-checked in main() at the bottom of this file).
//
// NOTE: If some of your code is not yet done, comment out the appropriate
//       #include files above and concepts checks (in main() at the bottom
//       of this file) in order to concept-check code you have completed.
//
// NOTE: Concepts are special compile-time predicates that are checked at
//       compile-time. This means any compile-computed expressions that
//       produce boolean values can be used within concepts, e.g.,
//       almost everything defined in <type_traits> can also be used.
//       The C++20 standard defines a number of concepts in <concepts>
//       as well.
//
// NOTE: Remember these checks only check some of the required 
//       interfaces. In particular, these checks do not check that your code
//       runs correctly. Also these checks are not exhaustive.
//
// See https://en.cppreference.com/w/cpp/concepts for C++20 <concepts> 
// definitions. See https://en.cppreference.com/w/cpp/header/type_traits for
//   <type_traits> definitions.
//
// NOTE: Know that concepts can be used within the definition of a template, 
//       e.g.,
//
//        template <typename T>
//        requires 
//          copy_constructible<T> 
//          && requires(T a, T b) { { a < b } -> convertible_to<bool> }
//        T mymin(T const& a, T const& b)
//        {
//          return (a < b) ? a : b;
//        }
//
//      if mymin() is called with a type T is not copy constructible or if 
//      T does not have a suitable < operator defined then the code will not
//      compile, e.g.,
//
//        mymin(3,2)  // okay
//
//        struct foo { };
//        mymin(foo{}, foo{}); // error, no < defined
//

//===========================================================================

using namespace std;

//===========================================================================

//
// NOTE: The ppm_pixel_concept corresponds to the required definitions in 
//       ppm_pixel.hxx.
//

template <typename PPMPixel>
concept ppm_pixel_concept =
  requires() 
  { 
    typename PPMPixel::component_type;
  }
  && same_as<typename PPMPixel::component_type, uint8_t>
  && requires(PPMPixel const& p) 
  { 
    { p.component_max() } -> convertible_to<uint8_t>; 
  }

  && default_initializable<PPMPixel>
  && copy_constructible<PPMPixel>
  && is_copy_assignable_v<PPMPixel> // i.e., see <type_traits>
  && constructible_from<
    PPMPixel, 
    typename PPMPixel::component_type const, 
    typename PPMPixel::component_type const, 
    typename PPMPixel::component_type const
  >
  && constructible_from<PPMPixel, typename PPMPixel::component_type const>

  && requires(
    PPMPixel const& cpa, PPMPixel const& cpb, PPMPixel& p, 
    istream& is, ostream& os, 
    bool const cb
  )
  {
    { cpa.save(os,cb) } -> same_as<ostream&>;
    { is >> p } -> same_as<istream&>;
    { os << cpa } -> same_as<ostream&>;
    { cpa == cpb } -> convertible_to<bool>;
    { cpa != cpb } -> convertible_to<bool>;
  }
;

//===========================================================================

//
// NOTE: The ppm_concept corresponds to the required definitions in ppm.hxx.
//

template <typename PPM>
concept ppm_concept =
  requires()
  {
    typename PPM::pixel_type;
    typename PPM::width_type;
    typename PPM::height_type;
  }
  && same_as<typename PPM::pixel_type, ppm_pixel>
  && same_as<typename PPM::width_type, size_t>
  && same_as<typename PPM::height_type, size_t>

  && !default_initializable<PPM>
  && copy_constructible<PPM>
  && move_constructible<PPM>
  && is_copy_assignable_v<PPM>
  && is_move_assignable_v<PPM>

  && constructible_from<
    PPM,
    typename PPM::width_type const&,
    typename PPM::height_type const&
  >

  && requires(
    PPM p, PPM const& cp, 
    typename PPM::width_type const& w, 
    typename PPM::height_type const& h,
    ostream& os, 
    bool const cb
  )
  {
    { cp.width() } -> same_as<size_t>;
    { cp.height() } -> same_as<size_t>;
    { p(w,h) } -> same_as<typename PPM::pixel_type&>;
    { cp(w,h) } -> same_as<typename PPM::pixel_type const&>;
    { cp.save(os) } -> same_as<ostream&>;
    { cp.save(os,cb) } -> same_as<ostream&>;
  }
;

//===========================================================================

//
// NOTE: The rgb_concept corresponds to the required definitions in rgb.hxx.
//

template <typename RGB>
concept rgb_concept =
  requires()
  {
    typename RGB::value_type;
  }
  && same_as<typename RGB::value_type, float>

  && default_initializable<RGB>
  && copy_constructible<RGB>
  && is_copy_assignable_v<RGB>
  
  && constructible_from<
    RGB,
    typename RGB::value_type const&,
    typename RGB::value_type const&,
    typename RGB::value_type const&
  >

  && requires(RGB const& c)
  {
    { to_ppm_pixel(c) } -> same_as<ppm_pixel>;
  }
;

//===========================================================================

//
// NOTE: The hsv_concept corresponds to the required definitions in hsv.hxx.
//

template <typename HSV>
concept hsv_concept =
  requires()
  {
    typename HSV::value_type;
  }
  && same_as<typename HSV::value_type, float>

  && default_initializable<HSV>
  && copy_constructible<HSV>
  && is_copy_assignable_v<HSV>

  && constructible_from<
    HSV,
    typename HSV::value_type const&,
    typename HSV::value_type const&,
    typename HSV::value_type const&
  >

  && requires(HSV const& c)
  {
    { hsv_to_rgb(c) } -> same_as<rgb>;
    { to_ppm_pixel(c) } -> same_as<ppm_pixel>;
  }
;

//===========================================================================

//
// NOTE: The evenly_divide_n_concepts corresponds to the required definitions
//       in utils.hxx.
//
template <typename OutIter, typename Value>
concept evenly_divide_n_concept =
  is_integral_v<Value>
  && is_arithmetic_v<Value>
  && requires(OutIter o, Value const& v, Value const& n)
  {
    { evenly_divide_n(o, v, n) } -> same_as<void>;
    { fill_n(o, n, v / n) }; // i.e., ensures o is valid as output iterator
  }
;

//===========================================================================

//
// NOTE: The mandelbrot_concept corresponds to the required definitions in
//       mandelbrot.hxx.
//
template <typename Real>
concept mandelbrot_concept =
  requires(
    complex<Real> const& c, 
    Real const& r,
    size_t const& s,
    ppm_pixel (*mandel2colour)(size_t const&),
    ppm const& image,
    string const& str
  )
  {
    { compute_mandelbrot_at(c,s) } -> same_as<size_t>;
    { mandelbrot_result_to_ppm_pixel(s,s) } -> same_as<ppm_pixel>;
    { image_index_to_real(s,s,r,r) } -> same_as<Real>;
    { write_cell_info_concurrently(s,s,s,s) } -> same_as<void>;
    { write_image(image,str) } -> same_as<void>;
    { draw_mandelbrot_set(image,c,c,mandel2colour,s) } -> same_as<void>;
  }
;

//
// NOTE: The mandelbrot_par_concept corresponds to the required definitions 
//       in mandelbrot_par.hxx.
//
template <typename Real>
concept mandelbrot_par_concept =
  requires(
    complex<Real> const& c, 
    size_t const& s,
    ppm_pixel (*mandel2colour)(size_t const&),
    ppm const& image
  )
  {
    { draw_mandelbrot_set_par(image,c,c,mandel2colour,s,s) } -> same_as<void>;
  }
;

//===========================================================================

int main()
{
  //
  // Apply concept checking using static_asserts...
  //
  static_assert(ppm_pixel_concept<ppm_pixel>, "ppm_pixel_concept has issues");
  static_assert(ppm_concept<ppm>, "ppm_concept has issues");
  static_assert(rgb_concept<rgb>, "rgb_concept has issues");
  static_assert(hsv_concept<hsv>, "hsv_concept has issues");
  static_assert( // check one output iterator and numeric type...
    evenly_divide_n_concept<std::vector<size_t>::iterator, size_t>, 
    "an evenly_divide_n_concept has issues"
  );
  static_assert(mandelbrot_concept<float>, "mandelbrot_concept has issues");
}

//===========================================================================
