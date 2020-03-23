//Noah Campbell (10453611)
//Keerthana Madhavan (104995097)
//Spencer Briguglio (103746720)
//Jykee John Pavo (104891924)
//Taqiuddin Farooqui (104073892)

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

  //Declare a static std::mutex variable
  static std::mutex mychoosing;

  {
    //std::lock_guard variable with mutex variable passed in constructor.
    std::lock_guard<std::mutex> lock(mychoosing);
    //Provided standard error message.
    std::cerr << "PROCESSING: (" << xlow << ',' << ylow << ")-(" << xhigh << ',' << yhigh << ") on thread " << std::this_thread::get_id() << '\n';
  }
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_write_par_hxx_
