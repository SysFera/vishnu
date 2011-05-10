#ifndef _BATCHFILE_H_
#define _BATCHFILE_H_

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#include <iterator>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>
#include <boost/shared_ptr.hpp>

#include "utilVishnu.hpp"

using namespace std;

static std::string prefix = "vishnu";
static const std::string group               = prefix+"_group";
static const std::string jobName             = prefix+"_job_name";
static const std::string jobOutput           = prefix+"_output";
static const std::string jobError            = prefix+"_error";
static const std::string jobWallClockLimit   = prefix+"_wallclocklimit";
static const std::string workingDir          = prefix+"_workingdir";
static const std::string notify_user         = prefix+"_notify_user";
static const std::string nodes               = prefix+"_job_nodes";
static const std::string queue               = prefix+"_queue";
static const std::string priority            = prefix+"_userpriority";
static const std::string mem                 = prefix+"_memory";
static const std::string cpuTime             = prefix+"_cputime";
static const std::string loadLevelerSec      = "loadleveler_sec";
static const std::string torqueSec           = "torque_sec";
static const std::string commandSec          = "command_sec";

class BatchConvertor {

  public :

    BatchConvertor();
    BatchConvertor(const int batchType, 
                   const std::string&  genericJob);

    int parseFile(std::string& errorMessage);

    std::string getJobDescriptor();

    void initializeTableOfSymbols(); 

    bool scriptIsGeneric();
 
    ~BatchConvertor();

  private:
    const int mbatchType;
    const std::string mgenericJob;
    std::vector< pair<std::string, std::string> > mjobDescriptor;
    std::map<std::string, std::string>  mconversionTable;
    std::vector<std::string> mtableOfSymbols;
    std::string mendScript;
};


boost::shared_ptr<BatchConvertor>  
vishnuJobConvertor(const int mbatchType, 
                   const std::string& file);

#endif
