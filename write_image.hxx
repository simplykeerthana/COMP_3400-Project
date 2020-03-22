#ifndef comp3400_2020w_project_write_image_hxx_
#define comp3400_2020w_project_write_image_hxx_

//===========================================================================

#include <string>                   // for std::string
#include <chrono>                   // for timing and clocks
#include <fstream>                  // for std::ofstream
#include <iostream>                 // for std::cerr and std::cout

//===========================================================================

template <typename Image>
void write_image(Image const& image, std::string const& fname)
{
  //
  // TODO: You must write the code in this function as described below.
  //
  // This function opens a file called fname, truncating any existing file
  // should one exist, and writes out the ppm image, i.e., image. to
  // that file. The time it takes to output the file in seconds is also 
  // computed using <chrono>'s std::chrono::high_resolution_clock::not()
  // and is output to standard output as follows:
  //
  //   << "OFSTREAM_TIME: " << fname << " took " << secs.count()
  //   << " seconds to write.\n";
  //
  // where secs is the number of seconds it to write the file.
  //
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_write_image_hxx_
