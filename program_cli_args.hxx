#ifndef comp3400_2020w_project_program_cli_args_hxx_
#define comp3400_2020w_project_program_cli_args_hxx_

//===========================================================================

#include <cstddef>    // for std::size_t
#include <string>     // for std::string
#include <vector>     // for std::vector
#include <thread>     // for std::thread::hardware_concurrency()

#if defined(PARALLEL) && defined(__GNUC__)
  // GCC uses Intel Parallel STL library which uses the Intel Threaded 
  // Building Blocks library to implement its C++17 parallel algorithms.
  #include <tbb/task_scheduler_init.h>
#endif

//===========================================================================

struct program_cli_args
{
  bool help;
  int exit_status;
  std::size_t image_width;
  std::size_t image_height;
  std::size_t iterations;
  std::size_t num_threads;
  std::size_t min_avoid_false_sharing_size_in_bytes;
  std::string outfname;
  std::vector<std::string> msgs;

  template <typename FwdIter>
  program_cli_args(FwdIter first, FwdIter last) :
    help(false),
    exit_status(0),
    image_width(256),
    image_height(256),
    iterations(64),
#if defined(PARALLEL) && defined(__GNUC__)
    num_threads(tbb::task_scheduler_init::default_num_threads()),
#else
    num_threads(std::thread::hardware_concurrency()),
#endif
    min_avoid_false_sharing_size_in_bytes(64), // typical cache line size
    outfname("mandelbrot.ppm")
  {
    using namespace std;

    std::string const program_name = *first;
    for (++first; first != last; ++first)
    {
      std::string arg(*first);
      if (arg == "--help")
      {
        help = true;
        ++first;
        exit_status = 1;
      }

      if (arg == "--width" || arg == "-w")
      {
        bool okay = false;
        if (next(first) != last)
        {
          try 
          { 
            image_width = stoul(*++first); 
            okay = (image_width > 0); 
          }
          catch (...) { }
        }

        if (!okay)
        {
          ostringstream buf;
          buf << "--width requires a positive integer argument\n";
          msgs.push_back(buf.str());
          exit_status = 10;
        }
        else
        {
          ostringstream buf;
          buf << "Setting width to " << image_width << '\n';
          msgs.push_back(buf.str());
        }
        continue;
      }

      if (arg == "--height" || arg == "-h")
      {
        bool okay = false;
        if (next(first) != last)
        {
          try 
          { 
            image_height = stoul(*++first); 
            okay = (image_height > 0); 
          }
          catch (...) { }
        }

        if (!okay)
        {
          ostringstream buf;
          buf << "--height requires a positive integer argument\n";
          msgs.push_back(buf.str());
          exit_status = 11;
        }
        else
        {
          ostringstream buf;
          buf << "Setting height to " << image_height << '\n';
          msgs.push_back(buf.str());
        }
        continue;
      }

#if defined(PARALLEL)
      if (arg == "--threads" || arg == "-t")
      {
        bool okay = false;
        if (next(first) != last)
        {
          try 
          {
            num_threads = clamp(size_t(stoul(*++first)), size_t{1}, num_threads);
            okay = true; 
          }
          catch (...) { }
        }

        if (!okay)
        {
          ostringstream buf;
          buf << "--threads requires a positive integer argument\n";
          msgs.push_back(buf.str());
          exit_status = 12;
        }
        else
        {
          ostringstream buf;
          buf << "Requesting " << num_threads << " threads.\n";
          msgs.push_back(buf.str());
        }
        continue;
      }
#endif
      
      if (arg == "--iterations" || arg == "-i")
      {
        bool okay = false;
        if (next(first) != last)
        {
          try 
          { 
            iterations = stoul(*++first); 
            okay = (iterations > 0); 
          }
          catch (...) { }
        }

        if (!okay)
        {
          ostringstream buf;
          buf << "--iterations requires a positive integer argument\n";
          msgs.push_back(buf.str());
          exit_status = 13;
        }
        else
        {
          ostringstream buf;
          buf << "Setting iterations to " << iterations << '\n';
          msgs.push_back(buf.str());
        }
        continue;
      }
      
      if (arg == "--out" || arg == "-o")
      {
        if (next(first) != last)
        {
          outfname = *++first;
          ostringstream buf;
          buf << "Setting output file name to " << outfname << '\n';
          msgs.push_back(buf.str());
        }
        else
        {
          ostringstream buf;
          buf << "--out requires a writable file/path name\n";
          msgs.push_back(buf.str());
          exit_status = 14;
        }
        continue;
      }

      // don't know what this argument is so...
      {
        ostringstream buf;
        buf
          << "\nUsage: " << program_name << " <options>\n"
             "   options:\n"
             "     --help            Outputs this help text.\n"
             "     -w <width>        Specifies width of image. Default: " << image_width << "\n"
             "     -h <height>       Specifies height of image. Default: " << image_height << "\n"
             "     -i <iterations>   Number of iterations to consider convergent. Default: " << iterations << "\n"
             "     -o <output_file>  Specifies output file. Default: " << outfname << "\n"
#if defined(PARALLEL)
             "     -t <num_threads>  Requests number of threads to use. Default: " << num_threads << "\n"
#endif
             "\n"
        ;
        msgs.push_back(buf.str());
        exit_status = 127;
        break;
      }
    }
  }
};

//===========================================================================

#endif // #ifndef comp3400_2020w_project_program_cli_args_hxx_
