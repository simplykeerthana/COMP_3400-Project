//Noah Campbell (10453611)
//Keerthana Madhavan (104995097)
//Spencer Briguglio (103746720)
//Jykee John Pavo (104891924)
//Taqiuddin Farooqui (104073892)

#ifndef comp3400_2020w_project_mandelbrot_par_hxx_
#define comp3400_2020w_project_mandelbrot_par_hxx_

//===========================================================================

#include <cmath>                    // for std::abs
#include <complex>                  // for std::complex

#include <string>                   // for std::string
#include <vector>                   // for std::vector

#include <algorithm>                // for various algorithms
#include <numeric>                  // e.g., for std::iota

#include <chrono>                   // for timing and clocks
#include <execution>                // for std::execution::par
#include <thread>                   // e.g., for std::thread::get_id()

#include <iostream>                 // for std::cerr and std::cout

#include "ppm.hxx"
#include "hsv.hxx"
#include "utils.hxx"
#include "program_cli_args.hxx"
#include "mandelbrot.hxx"

//===========================================================================

template <typename Image, typename Real, typename MandelToColourFunc>
void draw_mandelbrot_set_par(
  Image& image,
  std::complex<Real> const& min_point,
  std::complex<Real> const& max_point,
  MandelToColourFunc&& m2c,
  std::size_t const& max_iter,
  std::size_t chunks_per_x_axis
)
{

  //<-------------------------------------------------------Task 1-------------------------------------------------------------->

  //Width of the image
  auto const width = image.width();

  //<-------------------------------------------------------Task 2-------------------------------------------------------------->

  //Height of the image
  auto const height = image.height();

  //<-------------------------------------------------------Task 3-------------------------------------------------------------->

  //The "width" between the smallest and largest complex values i.e max_point - min_point
  auto const maxMinPointDifference = max_point - min_point;


  //<-------------------------------------------------------Task 4-------------------------------------------------------------->

  //Holds lambda function
  auto const compute_cell = [&](size_t const& ilow, size_t const& ihigh, size_t const& jlow, size_t const& jhigh) {
      
      //This will write out which cell is currently being computed (in parallel).
      write_cell_info_concurrently(ilow, jlow, ihigh, jhigh);

      //Outer for loop with i
      for (size_t i = ilow; i != ihigh; ++i)
      {
          Real const x = image_index_to_real(i, width, maxMinPointDifference.real(), min_point.real());
          ;
          //Inner for loop with j
          for (size_t j = jlow; j != jhigh; ++j)
          {
              Real const y = image_index_to_real(j, height, maxMinPointDifference.imag(), min_point.imag());
              ;
              std::complex<Real> const c(x, y);
              image(i, j) = m2c(compute_mandelbrot_at(c, max_iter), max_iter);
          }
      }
      
  };
  

  //<-------------------------------------------------------Task 5-------------------------------------------------------------->

  //Vectors for the lower x-axis(idivs_lo) and upper x-axis(idivs_hi)
  std::vector<std::size_t> idivs_lo;
  std::vector<std::size_t> idivs_hi;
 
  //Call evenly divide
  evenly_divide_n(back_inserter(idivs_lo), width, chunks_per_x_axis);
  std::inclusive_scan(begin(idivs_lo), end(idivs_lo), back_inserter(idivs_hi));
  std::exclusive_scan(begin(idivs_lo), end(idivs_lo), begin(idivs_lo), 0);



  //<-------------------------------------------------------Task 6-------------------------------------------------------------->

  //Repeat steps from task 5 for y-axis
  //Vectors for the lower y-axis(jdivs_lo) and upper y-axis(jdivs_hi)
  std::vector<std::size_t> jdivs_lo;
  std::vector<std::size_t> jdivs_hi;

  evenly_divide_n(back_inserter(jdivs_lo), height, chunks_per_x_axis);
  std::inclusive_scan(begin(jdivs_lo), end(jdivs_lo), back_inserter(jdivs_hi));
  std::exclusive_scan(begin(jdivs_lo), end(jdivs_lo), begin(jdivs_lo), 0);



  //<-------------------------------------------------------Task 7-------------------------------------------------------------->

  //Indices will hav the same size as idivs_lo with all container initialize to 0
  std::vector<std::size_t> indices(idivs_lo.size());

  //Using the iota to populate the indices
  std::iota(begin(indices), end(indices), 0);

  
  //<-------------------------------------------------------Task 8-------------------------------------------------------------->

  //Starting the timer
  auto start = std::chrono::high_resolution_clock::now();

  //Outer for loop
  for (int i = 0; i < indices.size(); i++)
  {
      //Inner for loop
      std::for_each(std::execution::par, begin(indices), end(indices), [&](std::size_t const& j) {

          //<-------------------------------------------------------Task 9-------------------------------------------------------------->
          //Calling the copute cell
          compute_cell(idivs_lo, idivs_hi, jdivs_lo, jdivs_hi);
	  
          });
  }

  //Get the end time
  auto end = std::chrono::high_resolution_clock::now();

  //Calculating the time it took for the process the mandelbrot image
  std::chrono::duration<double> secs = end - start;

  //Output information to the user
    std::cout << "MANDELBROT_TIME: " << width << " by " << height << " Mandelbrot set took " << secs.count() << " seconds.\n";

#endif // #ifndef comp3400_2020w_project_mandelbrot_par_hxx_
