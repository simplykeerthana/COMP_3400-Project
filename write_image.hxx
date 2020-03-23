//Noah Campbell (10453611)
//Keerthana Madhavan (104995097)
//Spencer Briguglio (103746720)
//Jykee John Pavo (104891924)
//Taqiuddin Farooqui (104073892)

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
    using namespace std;
    
    //Open file fname
    cout << "Writing to the file " << fname << '\n';
    cout.flush();
    
    auto time_start = chrono::high_resolution_clock::now();
    {
        ofstream out(fname, ios::binary);
        image.save(out, true);
    }
    
    auto time_end = chrono::high_resolution_clock::now();
    
    //Float type because we are calculating the time elasped
    chrono::duration<float> secs = time_end - time_start;
    
    
  cout  << "OFSTREAM_TIME: " << fname << " took " << secs.count()
       << " seconds to write.\n";
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_write_image_hxx_
