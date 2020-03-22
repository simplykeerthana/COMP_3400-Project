//===========================================================================

#include <iostream>                 // for std::cout
#include "hsv.hxx"

//===========================================================================

int main()
{
  using namespace std;

  for (float i={}; i < 361.F; i += 30.F)
  {
    cout 
      << i << ": " 
      << to_ppm_pixel(hsv_to_rgb({ i, 1.0F, 1.0F })) << "; "
      << to_ppm_pixel(hsv_to_rgb({ i, 1.0F, 0.5F })) << "; "
      << to_ppm_pixel(hsv_to_rgb({ i, 0.5F, 1.0F })) << "; "
      << to_ppm_pixel(hsv_to_rgb({ i, 0.5F, 0.5F })) << '\n'
    ;
  }
}

//===========================================================================
