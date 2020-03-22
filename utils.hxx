#ifndef comp3400_2020w_project_utils_hxx_
#define comp3400_2020w_project_utils_hxx_

//===========================================================================

#include <algorithm>

//===========================================================================

template <typename OutIter, typename Value>
void evenly_divide_n(OutIter out, Value const& value, Value const& n)
{
  using namespace std;

  auto const zero = Value{};
  auto const quotient = value / n;
  auto const remainder = value % n;

  fill_n(out, n, quotient);

  if (remainder != zero)
  {
    *out = remainder;
    ++out;
  }
}

//===========================================================================

#endif // #ifndef comp3400_2020w_project_utils_hxx_
