#ifndef __POSIXPARSER__HH__
#define __POSIXPARSER__HH__


#include <string>
#include <time.h>

class Definition {
  public:
    std::string key;
    std::string value;
};



class JobCtx {
  public:
   std::string vishnu_working_dir;
   std::string vishnu_job_name;
   std::string vishnu_output;
   std::string vishnu_error;
   ::time_t vishnu_wallclocklimit;
   int vishnu_nbNodesAndCpuPerNode;
   int vishnu_memory;

   void AddDefinition(Definition Current);
} ;



bool ParseCommand(char* Command,JobCtx& Context);
bool ParseString();

#endif
