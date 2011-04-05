#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"


#include "DIET_server.h"

#include "utilServer.hpp"

// definition of the number of available services and their names
#define NB_SERVICES 1

using namespace std;
using namespace vishnu;

static const char* SERVICES[NB_SERVICES] = {
  "jobSubmit_"
};

std::ostream&
operator<<(std::ostream& os, const TMS_Data::SubmitOptions_ptr& options) {

  if(options!=NULL) {
    std::string name = options->getName();
    std::string queuename = options->getQueue();
    int wallTime = options->getWallTime();
    int nbCpu = options->getNbCpu();
    int nbNodesAndCpuPerNode = options->getNbNodesAndCpuPerNode();
    std::string outputPath = options->getOutputPath();
    std::string errorPath =  options->getErrorPath();
    int mem = options->getMemory();

    os << "==============SubmitOptions content ================" << endl;
    if(name.size()!=0) os << setw(25) << right << "name: " << name  << endl;
    if(queuename.size()!=0) os << setw(25) << right << "queuename: " << queuename  << endl;
    if(wallTime!=0) os << setw(25) << right << "wallTime: " << wallTime  << endl;
    if(nbCpu!=-1) os << setw(25) << right << "nbCpu: " << nbCpu  << endl;
    if(nbNodesAndCpuPerNode!=-1) os << setw(25) << right << "nbNodesAndCpuPerNode: " << nbNodesAndCpuPerNode  << endl;
    if(outputPath.size()!=0) os << setw(25) << right << "outputPath: " << outputPath  << endl;
    if(errorPath.size()!=0)  os << setw(25) << right << "errorPath: " << errorPath  << endl;
    if(mem!=-1) os << setw(25) << right << "memory: " << mem  << endl;
    os << endl;
  }
  return os;
}

std::ostream&
operator<<(std::ostream& os, const TMS_Data::Job_ptr& job) {

  std::string jobId = job->getJobId();
  std::string jobScriptContent = job->getJobPath();

  os << "==============Job content ================" << endl;
  if(jobId.size()!=0) os << setw(25) << right << "jobId: " << jobId  << endl;
  if(jobScriptContent.size()!=0) os << setw(25) << right << "jobScriptContent: " << jobScriptContent  << endl;
  os << endl;

  return os;
}

/*
 * SOLVE FUNCTIONS
 */

/* submitJob */
int
solve_submitJob(diet_profile_t* pb)
{

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* options  = NULL;
  char* jobSerialized = NULL;
  char* updateJobSerialized = NULL;

  cout << "Solve submitJob " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &options, NULL);
  cout << "************options=" << options << " ..." << endl;
  diet_string_get(diet_parameter(pb,3), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  TMS_Data::SubmitOptions_ptr submitOptions = NULL;
  //To parse the object serialized
  if(!parseTMSDataObject(std::string(options), submitOptions)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  cout << submitOptions << endl;;

  TMS_Data::Job job;
  job.setJobId("JOB_1");
  job.setJobPath("/home/blala/toto.job");
  //To serialize the user object
  const char* name = "solveSubmitJob";
  ::ecorecpp::serializer::serializer _ser(name);
  updateJobSerialized = strdup(_ser.serialize(const_cast<TMS_Data::Job_ptr>(&job)).c_str());
  if (diet_string_set(diet_parameter(pb,4), updateJobSerialized, DIET_VOLATILE)) {
       cerr << "diet_string_set error" << endl;
       delete submitOptions;
       return 1;
  }

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  if (diet_string_set(diet_parameter(pb,5), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
      delete submitOptions;
    return 1;
  }

  delete submitOptions;
  cout << " done" << endl;
 return 0;
}

/*
 * MAIN
 *
 * @param argc
 * @param argv
 *   argv[0] :
 */

int
main(int argc, char* argv[], char* envp[])
{

  int res = 0;
  diet_profile_desc_t* profile = NULL;

  if (argc < 3) {
    cerr << "Usage: ./" << argv[0] << " <cfg> <machineId>" << endl;
    return 1;
  }
  char* machineId = argv[2];

  // initialization of the service table
  diet_service_table_init(NB_SERVICES);
  /* connect */
  profile = diet_profile_desc_alloc((SERVICES[0]+std::string(machineId)).c_str(), 3, 3, 5);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,5), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_submitJob)) return 1;

  diet_profile_desc_free(profile);
  diet_print_service_table();

  res = diet_SeD(argv[1], argc, argv);

  /* Not reached */
 return res;
}

