//Noah Campbell (10453611)
//Keerthana Madhavan (104995097)
//Spencer Briguglio (103746720)
//Jykee John Pavo (104891924)
//Taqiuddin Farooqui (104073892)

#ifndef comp3400_2020w_project_ppm_pixel_save_hxx_
#define comp3400_2020w_project_ppm_pixel_save_hxx_

//===========================================================================

  std::ostream& save(std::ostream& os, bool binary = false) const
  {
      //If binary == true, then each component is output as a character value.
      if(binary == true)
      {
          os.put(r).put(g).put(b);
      }

      //If binary == false, then each component is output as an ASCII base
      if(binary == false)
      {
          //PPM pixel output in RGB order     
          os << static_cast<std::size_t>(r) << "" << static_cast<std::size_t>(g) << "" << static_cast<std::size_t>(b);
      }
      
    //
    return os;
  }

//===========================================================================

#endif // #ifndef comp3400_2020w_project_ppm_pixel_save_hxx_
