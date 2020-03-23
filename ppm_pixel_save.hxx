#ifndef comp3400_2020w_project_ppm_pixel_save_hxx_
#define comp3400_2020w_project_ppm_pixel_save_hxx_

//===========================================================================

  std::ostream& save(std::ostream& os, bool binary = false) const
  {
    //
    // TODO: You must write the code in this function as described below.
    //
    // This function writes *this, i.e., a ppm_pixel to os.
    //
    // Each ppm_pixel is output as follows:
    //
    //   * its r (red) component is output first,
    //   * its g (green) component is output second, and
    //   * its b (blue) component is output third.
    //
    // If binary == true, then each component is output as a character value.
    //   * Use the unformatted call os.put(c); to output the character value.
      
      if(binary == true)
      {
          os.put(r).put(g).put(b);
      }
    //
    // If binary == false, then each component is output as an ASCII base
    // 10 number.
    //   * Recall the trick used in your assignment to do this. If you
    //     simply output the character value it will write a single
    //     char value --not a base 10 integer.
      
      if(binary == false)
      {
          
          os << static_cast<std::size_t>(r) << "" << static_cast<std::size_t>(g) << "" << static_cast<std::size_t>(b);
      }
      
    //
    return os;
  }

//===========================================================================

#endif // #ifndef comp3400_2020w_project_ppm_pixel_save_hxx_
