#ifndef comp3400_2020w_project_ppm_hxx_
#define comp3400_2020w_project_ppm_hxx_

//===========================================================================

#include <cstddef>      // for std::size_t
#include <ostream>      // for std::ostream
#include <vector>       // for std::vector

#include "ppm_pixel.hxx"

//===========================================================================

class ppm
{
public:
  using pixel_type = ppm_pixel;
  using width_type = std::size_t;
  using height_type = std::size_t;

private:
  std::vector<pixel_type> image_;
  width_type width_;
  height_type height_;

public:
  ppm() = delete;
  ppm(ppm const&) = default;
  ppm(ppm&&) = default;
  ppm& operator =(ppm const&) = default;
  ppm& operator =(ppm&&) = default;

  ppm(width_type const& w, height_type const& h) :
    image_(w*h),
    width_{w},
    height_{h}
  {
  }

  size_t width() const
  {
    return width_;
  }

  size_t height() const
  {
    return height_;
  }

  pixel_type& operator()(width_type const& x, height_type const& y)
  {
    return image_[y*width_+x];
  }

  pixel_type const& operator ()(width_type const& x, height_type const& y) const
  {
    return image_[y*width_+x];
  }

  std::ostream& save(std::ostream& os, bool binary=false) const
  {
    if (os)
    {
      os 
        << (binary ? "P6\n" : "P3\n")
        << width_ << ' ' << height_ << '\n'
        << "255\n"
      ;
      for (height_type j{}; j != height_; ++j)
      {
        for (width_type i{}; i != width_; ++i)
        {
          if (binary)
          {
            auto& pixel = (*this)(i,j);
            os.put(pixel.r).put(pixel.g).put(pixel.b);
          }
          else
            os << (*this)(i,j) << '\n';
        }
      }
    }
    return os;
  }
};

//===========================================================================

#endif // #ifndef comp3400_2020w_project_ppm_hxx_
