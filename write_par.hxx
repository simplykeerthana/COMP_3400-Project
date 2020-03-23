#ifndef comp3400_2020w_project_write_par_hxx_
#define comp3400_2020w_project_write_par_hxx_

//===========================================================================

#include <cstddef>  // for std::size_t
#include <mutex>    // for std::mutex
#include <thread>   // for std::thread
#include <iostream> // for std::cerr

//===========================================================================

inline void write_cell_info_concurrently(
    size_t const &xlow, size_t const &ylow,
    size_t const &xhigh, size_t const &yhigh)
{

  static std::mutex mychoosing;

  {
    std::lock_guard<std::mutex> lock(mychoosing);
    std::cerr << "PROCESSING: (" << xlow << ',' << ylow << ")-(" << xhigh << ',' << yhigh << ") on thread " << std::this_thread::get_id() << '\n';
  }

  //
  // TODO: You must write the code in this function as described below.
  //
  // 1) Declare a static std::mutex variable with a name of your choosing.
  //
  // 2) Open a code block and place the following instructions inside:
  //
  //   2a) Declare a std::lock_guard variable. Pass the std::mutex
  //       variable you declared in step 1 to its constructor.
  //
  //   2b) Write the following to standard error:
  //         << "PROCESSING: (" << xlow << ',' << ylow
  //         << ")-(" << xhigh << ',' << yhigh << ") on thread "
  //         << this_thread::get_id() << '\n';
  //
  // and close the code block.
  //
  // NOTE: Look up std::mutex and std::lock_guard on cppreference.com.
  //
  //       Look at the example code associated with each.
  //
  //       Since this is the only code that will be outputting to
  //       an IOStream in parallel, it is acceptable to declare
  //       the std::mutex variable in this function (instead as a
  //       global variable).
  //
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_write_par_hxx_
