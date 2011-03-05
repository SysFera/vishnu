#include <sys/time.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>

/*To get the time in second*/
inline double gettime()
{
   struct timeval tmp ;
   gettimeofday (&tmp, 0) ;

   // real time 
   return (double) tmp.tv_sec + ((double) tmp.tv_usec) * 1e-6;
}

/*To print the execution of the program*/
void print_ssh_time(double t, std::string prog)
{
  std::cout << ":-----------------------------------------" << std::endl;
  std::cout << "Execution of " << prog << std::endl;
  std::cout << ": ssh_time = " << t << " s" << std::endl;
  std::cout << ":-----------------------------------------" << std::endl;
}

int main(int argc, char* argv[]) {

   if(argc < 2) {
     std::cout << "usage : " << argv[0] << "ssh_progname>" << std::endl;
     return 1;
   }
   char* ssh_progname = argv[1];
   double t0;
   double t1;
   std::ostringstream os;
   os << "ssh localhost "; 
   os << ssh_progname;

   t0 = gettime();
     //Test ssh 
     system(os.str().c_str());
   t1 = gettime(); 
    print_ssh_time(t1-t0, os.str());

 return 0;
}
