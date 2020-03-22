//===========================================================================

#include <sstream>                  // for std::stringstream
#include <fstream>                  // for std::ofstream
#include <iostream>                 // for std::cerr and std::cout

#include "ppm_pixel.hxx"

//===========================================================================

int main()
{
  using namespace std;

  ppm_pixel const a;
  ppm_pixel const b(127); // gray
  ppm_pixel const c(b);
  ppm_pixel const d(255,255,0); // yellow
  ppm_pixel e;
  e = d;

  cout 
    << "a: " << a << '\n'
    << "b: " << b << '\n'
    << "c: " << c << '\n'
    << "d: " << d << '\n'
    << "e: " << e << "\n\n"
  ;

  cout << "e == d: " << (e == d) << '\n';
  cout << "e != d: " << (e != d) << "\n\n";

  cout << "e == c: " << (e == c) << '\n';
  cout << "e != c: " << (e != c) << "\n\n";

  cout << "c == b: " << (c == b) << '\n';
  cout << "c != b: " << (c != b) << "\n\n";

  ostringstream out;
  out << e << ' ' << c << ' ' << a << '\n';
  cout << out.str();

  ppm_pixel x(255), y(254), z(253); // using different values
  istringstream in(out.str());
  if (in >> x >> y >> z)
  {
    cout 
      << "x: " << x << '\n'
      << "y: " << y << '\n'
      << "z: " << z << "\n\n"
    ;
  
    if (e == x && c == y && a == z)
      cout << "success!\n";
    else
      cout << "failure!\n";
  }
  else
    cout << "critical failure!\n";
}

//===========================================================================
