//===========================================================================

#include <cmath>                    // for std::abs
#include <complex>                  // for std::complex

#include <string>                   // for std::string
#include <vector>                   // for std::vector

#include <algorithm>                // for various algorithms
#include <iterator>                 // for std::ostream_iterator
#include <chrono>                   // for timing and clocks
#include <fstream>                  // for std::ofstream
#include <iostream>                 // for std::cerr and std::cout

#include "ppm_pixel.hxx"
#include "ppm.hxx"
#include "rgb.hxx"
#include "hsv.hxx"
#include "utils.hxx"
#include "program_cli_args.hxx"

#if defined(SEQUENTIAL)
  #include "mandelbrot.hxx"
#elif defined(PARALLEL)
  #include "mandelbrot_par.hxx"
#else
  #error "Either the SEQUENTIAL or PARALLEL macro must be defined."
#endif


//===========================================================================

int main(int argc, char *argv[])
{
  using namespace std;

  using real_type = float;

  program_cli_args const ps(argv, argv+argc);

  // handle when invalid command line arguments are provided...
  copy(
    begin(ps.msgs), end(ps.msgs),
    ps.exit_status 
      ? ostream_iterator<string>(cerr) // if error exit status
      : ostream_iterator<string>(cout) // if no error exit status
  );

  // and terminate if needed...
  if (ps.exit_status)
    return ps.exit_status;

#if defined(PARALLEL)
#if defined(__GNUC__)
  // Set the maximum number of threads to schedule with Intel TBB...
  // NOTE: Currently there is no way to do this in ISO C++.
  tbb::task_scheduler_init internal_scheduler(ps.num_threads);
#endif
#endif // #ifdef PARALLEL

#if defined(PARALLEL)
  size_t const chunk = ps.min_avoid_false_sharing_size_in_bytes;
  size_t const smallest_pixel_run_per_chunk = 
    chunk / sizeof(ppm_pixel) + (chunk % sizeof(ppm_pixel) != 0)
  ;
  size_t const chunks_per_x_axis =  
    ps.image_width / smallest_pixel_run_per_chunk
    + (ps.image_width % smallest_pixel_run_per_chunk != 0)
  ;
  size_t const use_num_threads = min(chunks_per_x_axis, ps.num_threads);

  cout 
    << "INFO: Using " << use_num_threads 
    << " thread(s) breaking work into " << chunks_per_x_axis 
    << " chunk(s) per axis.\n";
#endif

  ppm image(ps.image_width, ps.image_height);
#if defined(SEQUENTIAL)
  draw_mandelbrot_set(
    image,
    complex<real_type>(-2.F, -1.5F), complex<real_type>(1.F, 1.5F), 
    mandelbrot_result_to_ppm_pixel,
    ps.iterations
  );
#elif defined(PARALLEL)
  draw_mandelbrot_set_par(
    image,
    complex<real_type>(-2.F, -1.5F), complex<real_type>(1.F, 1.5F), 
    mandelbrot_result_to_ppm_pixel,
    ps.iterations,
    chunks_per_x_axis
  );
#else
  #error "Either the SEQUENTIAL or PARALLEL macro must be defined."
#endif

  write_image(image, ps.outfname);
}

//===========================================================================

