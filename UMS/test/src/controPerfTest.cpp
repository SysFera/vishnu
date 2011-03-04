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
void print_vishnu_time(double t, std::string prog)
{
  std::cout << ":-----------------------------------------" << std::endl;
  std::cout << "Execution of " << prog << std::endl;
  std::cout << ": vishnu_time = " << t << " s" << std::endl;
  std::cout << ":-----------------------------------------" << std::endl;
}

int main(int argc, char* argv[]) {

   if(argc < 3) {
     std::cout << "usage : " << argv[0] << "<vishnu_connet pathToBin> <vishnu_progname>" << std::endl;
     return 1;
   }
   char* vishnu_connect_bin_path = argv[1];
   char* vishnu_progname = argv[2];
   double t0;
   double t1;
   std::ostringstream os1;
   os1 << vishnu_connect_bin_path << " root -w vishnu_user;";
   os1 << vishnu_progname;
   
   t0 = gettime();  
     //To execute vishnu programm 
     system(os1.str().c_str());
   t1 = gettime();
   //print_vishnu_time(t1-t0, os1.str());
 
   double vishnu_wall_time = t1-t0; 
   std::ostringstream os2;
   os2 << vishnu_connect_bin_path << " root -w vishnu_user;"; 
   t0 = gettime();
     //To get vishnu_connect time
     system(os2.str().c_str());
   t1 = gettime(); 
   print_vishnu_time(t1-t0, "vishnu_connect");
   print_vishnu_time(vishnu_wall_time-(t1-t0), vishnu_progname);
   
 return 0;
}
