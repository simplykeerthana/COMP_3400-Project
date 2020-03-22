//===========================================================================

#include <fstream>                  // for std::ofstream
#include <iostream>                 // for std::cerr and std::cout

#include "ppm.hxx"

//===========================================================================

int main()
{
  using namespace std;

  ppm image(10,10);
  for (size_t i{}; i != image.width(); ++i)
  {
    for (size_t j{}; j != image.height(); ++j)
    {
      image(i,j) = ppm_pixel(255,127,0);
    }
    image(i,i) = ppm_pixel(64,64,64);
  }

  {
    ofstream out("p3-test.ppm");
    image.save(out);
  }
  
  {
    ofstream out("p6-test.ppm", ios::binary);
    image.save(out, true);
  }
}

//===========================================================================
