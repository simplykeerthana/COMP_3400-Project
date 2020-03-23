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

  auto const compute_cell = [&](size_t const& ilow, size_t const& ihigh, size_t const& jlow, size_t const& jhigh) {
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
 

  evenly_divide_n(back_inserter(idivs_lo), width, chunks_per_x_axis);
  std::inclusive_scan(begin(idivs_lo), end(idivs_lo), back_inserter(idivs_hi));
  std::exclusive_scan(begin(idivs_lo), end(idivs_lo), begin(idivs_lo), 0);



  //<-------------------------------------------------------Task 6-------------------------------------------------------------->

  //Vectors for the lower y-axis(jdivs_lo) and upper y-axis(jdivs_hi)
  std::vector<std::size_t> jdivs_lo;
  std::vector<std::size_t> jdivs_hi;

  evenly_divide_n(back_inserter(jdivs_lo), height, chunks_per_x_axis);
  std::inclusive_scan(begin(jdivs_lo), end(jdivs_lo), back_inserter(jdivs_hi));
  std::exclusive_scan(begin(jdivs_lo), end(jdivs_lo), begin(jdivs_lo), 0);



  //<-------------------------------------------------------Task 7-------------------------------------------------------------->

  //indices will hav the same size as idivs_lo with all container initialize to 0
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

  

	
	
  //
  // TODO: You must write the code in this function as described below.
  //
  // This function computes computes the Mandelbrot set in parallel, i.e.,
  // it computes what the draw_mandelbrot_set() function does sequentially
  // (although this function outputs additional data to standard error).
  //
  // The code need in this function is described in steps below.
  //
  // 1) Declare an "auto const" variable to store what image.width() returns.
  //
  // 2) Declare an "auto const" variable to store what image.height() returns.
  //
  // 3) Declare an "auto const" variable to store max_point - min_point,
  //    i.e., the "width" between the smallest and largest complex values.
  //    (This is the "width" of what is being computed.)
  //
  // NOTE: The Mandelbrot set's real axis is being plotted as the x-axis and
  //       the imagimary axis is being plotted as the y-axis. You'll note
  //       code below that maps image's integer pixel coordinates to a
  //       complex value between min_point and max_point.
  //
  // TASK: The function arguments and the above three variables will be used
  //       in a large lambda function (it is easier to write this way) that
  //       will do what draw_mandelbrot_set() does --except for a small
  //       square (i.e., cell) in the image. This lambda function will be 
  //       stored in a variable. Later in this function, the indices of each 
  //       cell will be computed. Once those are computed, you will write
  //       a parallel for_each loop inside for loop that will call the
  //       lambda function to compute the Mandelbrot set in parallel.
  //
  // 4) Declare an "auto const" variable called "compute_cell" to hold
  //    the lambda function described below. Remember that a const
  //    variable must be assigned a value!
  //
  //      NOTE: Normally one does not assign a lambda variable. You are
  //            doing this here so you can define a function inside
  //            draw_mandelbrot_set_par() that uses (by reference)
  //            the variables and arguments in draw_mandelbrot_set_par().
  //
  //  4a) The lambda should start with:
  //
  //        [&](size_t const& ilow, size_t const& ihigh, 
  //            size_t const& jlow, size_t const& jhigh)
  //        {
  //
  //      i.e., which captures variables/arguments by reference, and,
  //      passes in the "lower point" (ilow,jlow) and the
  //      "higher point" (ihigh,jhigh).
  //
  //  4b) Call write_cell_info_concurrently(ilow, jlow, ihigh, jhigh);
  //
  //        * This will write out which cell is currently being computed
  //          (in parallel).
  //
  //        * This is useful to have in the code to debug what is happening
  //          (or not) in parallel. If one makes a mistake computing
  //          the cell boundaries or with some loops, it is easy to
  //          have code that doesn't compute all cells or loops infinitely.
  //          Outputing the cells being computed helps determine what is
  //          going on.
  //
  //  4c) Write and "convert" the i and j for loops in draw_mandelbrot_set()
  //      as follows:
  //
  //        * i must start at ilow,
  //        * i's for loop will terminate at ihigh,
  //        * j must start at jlow,
  //        * j's for loop will terminate at jhigh.
  //
  //   4d) Close off the lambda function and write the semicolon for the
  //       compute_cell variable.
  //
  // TASK: Now you want to divide the image into square cells so a for loop
  //       can be used to compute each cell in parallel (up to the core limit
  //       of the computer this is being run on). This will be done using
  //       two vectors (one for the "lower" cell values and one for the
  //       "high" (upper) cell values), and these C++ Standard Library
  //       functions:
  //
  //         * std::inclusive_scan()
  //         * std::exclusive_scan()
  //
  //       Then you will place sequential integer values into a vector
  //       from 0 to the number of cells in each vector above. (This is
  //       so one can "for loop" through all cells in parallel.) This
  //       involves calling the Standard Library function iota().
  //
  // NOTE: Look up and understand what inclusive_scan(), exclusive_scan(),
  //       and iota() do on cppreference.com.
  //
  // 5) Compute the lower x-axis and upper x-axis values for each cell.
  //
  //      * Remember the x-axis corresponds to the "i" variables used in this
  //        function.
  //
  //   5a) Declare two std::vector variables that holds std::size_t values.
  //       Call one variable "idivs_lo" and the other "idivs_hi".
  //
  //   5b) Call evenly_divide_n() passing it three arguments as follows:
  //         * Arg1: Pass what back_inserter(idivs_lo) returns.
  //         * Arg2: Pass the image's (full) width.
  //         * Arg2: Pass the chunks_per_x_axis.
  //
  // NOTE: Look at the evenly_divide_n() code provided and understand what
  //       it does. What is it populating the idivs_lo vector with?
  //
  //   5c) Call std::inclusive_scan() passing it these arguments:
  //         * Arg1: begin(idivs_lo)
  //         * Arg2: end(idivs_lo)
  //         * Arg3: back_inserter(idivs_hi).
  //
  // NOTE: Look at std::inclusive_scan() on cppreference.com and understand 
  //       what it does. What is it populating the idivs_hi vector with?
  //       How is this occurring?
  //
  //   5d) Call std::exclusive_scan() passing it these arguments:
  //         * Arg1: begin(idivs_lo)
  //         * Arg2: end(idivs_lo)
  //         * Arg3: begin(idivs_lo)
  //         * Arg4: 0
  //
  // NOTE: Look at std::exclusive_scan() on cppreference.com and understand 
  //       what it does. What is it populating the idivs_lo vector with?
  //       How is this occurring?
  //
  // 6) Do the steps 5 for the y-axis by:
  //      * Changing all "i"s to "j"s.
  //      * Leave "chunks_per_x_axis" alone as each cell is square.
  //      * This is 4 lines of code (like steps 5).
  //
  // TASK: Now you have each cell's lower left and upper right corners
  //       computed. In order to run things in parallel with a Standard
  //       Library parallel algorithm, one needs a container with 
  //       indices to each cell in the above vectors. This will be generated
  //       with std::iota().
  //
  // 7a) Declare a std::vector holding std::size_t elements and give it a 
  //     name. Construct it with the value of idivs_lo.size(). (This will
  //     cause it to default construct that make std::size_t elements
  //     in the vector. See cppreference.com's documentation on 
  //     std::vector's constructors.)
  //
  // 7b) Call std::iota passing in the beginning of the vector in step 7a,
  //     the end of the vector in step7a, using an initial value of 0 (zero).
  //
  // TASK: Now that you have (from step 7) a vector whose elements are the
  //       indices of the "lo" and "hi" vectors from steps 5 and 6, the
  //       Mandelbrot set can be computed in parallel as instructed below.
  //
  // 8a) You are required to calculate the time required to compute the 
  //     Mandelbrot using std::chrono::high_resolution_clock::not().
  //     Record the starting time.
  //
  // 8b) Write a for loop to iterate through all elements of the vector of
  //     indices in step 7.
  //
  // 8c) Within the for loop in step 8b, call std::for_each as follows:
  //       * Arg1: std::execution::par
  //           i.e., this will be the parallel algorithm version
  //                 of std::for_each
  //       * Arg2 and Arg3: pass the begin and end of the vector
  //           of indices (i.e., from step 7). (All of the vectors in this 
  //           function are the same length --so using the same vector of 
  //           indices makes sense here.)
  //       * Arg4: pass in the lambda function described in step 8d
  //           below.
  //       * The for loop variable storing the "index" will be called "i" 
  //         here. You can code the for loop as you see fit --but know that
  //         "i" refers to the current iteration's integer index value 
  //         herein.
  //
  // 8d) The lambda function for the fourth argument in step 8c must be
  //     as written as follows:
  //       * capture all variables/symbols declared outside in the function
  //         by reference,
  //           * Hint: This requires an "&" in square brackets.
  //       * the lambda function must accept exactly one std::size_t const& 
  //         argument,
  //           * This argument is called "j" and is an "inner" for loop.
  //             (So step 8c is the "outer" for loop using "i"; step 8d
  //             is an "inner for loop" using "j". Like "i", "j"'s value
  //             is the current iteration's integer index value.)
  //       * the body of the lambda function calls the "compute_cell"
  //         lambda function defined in steps 4 as outlined in step 9
  //         below.
  //
  // 8e) Close the "inner" and "outer" for loops.
  //
  // 8f) Record the end time.
  //
  // 8g) Compute the different in time (in seconds) and output such
  //     to standard output as follows:
  //
  //       "MANDELBROT_TIME: " << image_width << " by " << image_height 
  //       << " Mandelbrot set took " << secs.count() << " seconds.\n"
  //
  //     where secs is the difference in times variable.
  //
  // 9) Calling compute_call requires passing in the correct arguments.
  //    Since the "outer" and "inner" for loops store indices
  //    to the idivs_lo, idivs_hi, jdivs_lo, and jdivs_hi vector,
  //    you need to compute the actual "iterator" positions of the
  //    data for each cell in the ?divs_lo and ?divs_hi vectors.
  //
  //    This is easily doable: std::vector iterators are random-access
  //    iterators. This means one can perform (simple) arithemtic
  //    operations with them to efficiently obtain specific 
  //    iterator positions. (Internally std::vector's iterators are
  //    very likely pointers since std::vector is stored as an array.)
  //
  //    The easiest way to get the correct values for the compute_cell()
  //    call is to obtain the beginning iterator of the appropriate 
  //    ?divs_?? vector, add as is appropriate the index i or j to
  //    that iterator, and then use operator * to point to the resulting
  //    iterator. (You will also need to use parentheses since * has higher
  //    priority than addition.)
  //
  //    The four arguments for compute_call() involve using these values:
  //
  //      * Arg1: idivs_lo and i
  //      * Arg2: idivs_hi and i
  //      * Arg3: jdivs_lo and j
  //      * Arg4: jdivs_hi and j
  //
  //    which correspond to the lower left and top right corners of each 
  //    cell.
  //
  //    Write the code to properly call compute_cell() and test your code.
  //
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_mandelbrot_par_hxx_
