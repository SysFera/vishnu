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
     std::cout << "usage : " << argv[0] << "<vishnu_progname> <ssh_progname>" << std::endl;
     return 1;
   }
   char* vishnu_progname = argv[1];
   char* ssh_progname = argv[2];
   double t0;
   double t1;
   std::ostringstream os1;
   os1 << "./vishnu_connect root;";
   os1 << vishnu_progname;
   os1 << " > testVishnu.txt";
   std::ostringstream os2;
   os2 << "ssh localhost "; 
   os2 << ssh_progname;
   os2 << " > testSsh.txt";

   t0 = gettime();  
     //To execute vishnu programm 
     system(os1.str().c_str());
   t1 = gettime();
   print_vishnu_time(t1-t0, os1.str());
  
   std::ostringstream os3;
   os3 << "./vishnu_connect root;"; 
   t0 = gettime();
     //To get vishnu_connect time
     system(os3.str().c_str());
   t1 = gettime(); 
   print_vishnu_time(t1-t0, "./vishnu_connect root");

   t0 = gettime();
     //Test ssh 
     system(os2.str().c_str());
   t1 = gettime(); 
    print_ssh_time(t1-t0, os2.str());

 return 0;
}
