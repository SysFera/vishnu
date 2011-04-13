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
#include "emfTMSUtils.hpp"

#include "DIET_server.h"
#include "DIET_Dagda.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "TorqueServer.hpp"

// definition of the number of available services and their names
#define NB_SERVICES 9

using namespace std;
using namespace vishnu;

static const char* SERVICES[NB_SERVICES] = {
  "jobSubmit_",
  "jobCancel_",
  "getListOfJobs_",
  "getJobsProgression_",
  "TMSMachineGetListOfQueues_",
  "TMSMachineRefreshPeriodSet_",
  "TMSMachineRefreshPeriodGet_",
  "jobOutPutGetResult_",
  "jobOutPutGetAllResult_"
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
    if(wallTime!=-1) os << setw(25) << right << "wallTime: " << wallTime  << endl;
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

int jobSubmit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job,
                  BatchServer* batchServer) {
    return batchServer->submit(scriptPath, options, job);
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
  char* scriptContent  = NULL;
  char* options  = NULL;
  char* jobSerialized = NULL;
  char* updateJobSerialized = NULL;
  std::string scriptPath = std::string(getenv("HOME"))+"/submit.pbs";

  cout << "Solve submitJob " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &scriptContent, NULL);
  cout << "************scriptContent=" << scriptContent << " ..." << endl;
  diet_string_get(diet_parameter(pb,3), &options, NULL);
  cout << "************options=" << options << " ..." << endl;
  diet_string_get(diet_parameter(pb,4), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  TMS_Data::SubmitOptions_ptr submitOptions = NULL;
  //To parse the object serialized

  if(!parseTMSEmfObject(std::string(options), submitOptions)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  cout << submitOptions << endl;;

  TMS_Data::Job_ptr job = NULL;
  if(!parseTMSEmfObject(std::string(jobSerialized), job)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  std::ofstream ofile(scriptPath.c_str());
  cout << "**************content of script *********" << endl;
  cout << scriptContent << endl;
  cout << "******************************************" << endl;
  ofile << scriptContent;
  ofile.close();

  TorqueServer* torqueServer = new TorqueServer();
  jobSubmit(scriptPath.c_str(),  *submitOptions, *job, torqueServer);

  //To serialize the user object
  const char* name = "solveSubmitJob";
  ::ecorecpp::serializer::serializer _ser(name);
  updateJobSerialized = strdup(_ser.serialize(job).c_str());

  if (diet_string_set(diet_parameter(pb,5), updateJobSerialized, DIET_VOLATILE)) {
       cerr << "diet_string_set error" << endl;
       delete submitOptions;
       return 1;
  }

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  if (diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
      delete submitOptions;
    return 1;
  }

  delete submitOptions;
  delete torqueServer;
  delete job;
  cout << " done" << endl;
 return 0;
}

/*
 * SOLVE FUNCTIONS
 */

/* cancelJob */
int
solve_cancelJob(diet_profile_t* pb)
{

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* jobSerialized = NULL;

  cout << "Solve cancelJob " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  TMS_Data::Job_ptr job = NULL;
  //To parse the object serialized

  if(!parseTMSEmfObject(std::string(jobSerialized), job)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  std::cout << "JobId:"<< job->getJobId() << std::endl;

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  if (diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
      delete job;
    return 1;
  }

  delete job;
  cout << " done" << endl;
 return 0;
}


/**
* \brief Function to solve the service solveGenerique
* \fn int solveGenerique(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
template <class QueryParameters, class List>
int
solveGenerique(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* optionValueSerialized = NULL;
  std::string listSerialized = "From TestServer";
  std::string empty = "";
  std::string errorInfo;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &optionValueSerialized, NULL);
  cout << "************optionValueSerialized=" << optionValueSerialized << " ..." << endl;

  //OUT Parameter
  diet_string_set(diet_parameter(pb,3), strdup(listSerialized.c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(pb,4), strdup(empty.c_str()), DIET_VOLATILE);

  return 0;
}

int
solve_ListOfJobs(diet_profile_t* pb) {
  return solveGenerique<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>(pb);
}

int
solve_getJobsProgression(diet_profile_t* pb) {
  return solveGenerique<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(pb);
}

/*
 * SOLVE FUNCTIONS
 */

/* listQueues */
int
solve_TMSMachineGetListOfQueues(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  std::string listSerialized = "From TestServer";

  cout << "Solve ListQueues " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  diet_string_set(diet_parameter(pb,2), strdup(listSerialized.c_str()), DIET_VOLATILE);

  if (diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
    return 1;
  }

  cout << " done" << endl;
  return 0;
}

/* SetRefreshPeriod */
int
solve_TMSMachineRefreshPeriodSet(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* newMachineRefreshPeriod = NULL;

  cout << "Solve TMSMachineRefreshPeriodSet " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &newMachineRefreshPeriod, NULL);
  cout << "************newMachineRefreshPeriod=" << newMachineRefreshPeriod << " ..." << endl;

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  if (diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
    return 1;
  }

  cout << " done" << endl;
  return 0;
}

/* getMachineRefreshPeriod */
int
solve_TMSMachineRefreshPeriodGet(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  std::string refreshPeriod = "10";

  cout << "Solve TMSMachineRefreshPeriodGet " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  diet_string_set(diet_parameter(pb,2), strdup(refreshPeriod.c_str()), DIET_VOLATILE);
  if (diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
    return 1;
  }
  cout << " done" << endl;
  return 0;
}

/* jobOutPutGetResult */
int
solve_jobOutPutGetResult(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* jobResultSerialized = NULL;
  char* jobResultOutSerialized;
  std::string empty = "";
  std::string errorInfo;

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &jobResultSerialized, NULL);
  cout << "************jobResultSerialized=" << jobResultSerialized << " ..." << endl;

  TMS_Data::JobResult_ptr jobResult = NULL;
  if(!parseTMSEmfObject(std::string(jobResultSerialized), jobResult)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  std::cout << jobResult->getJobId() << std::endl;
  jobResult->setErrorPath("/tmp/toto/");

  //To serialize the user object
  const char* name = "getResult";
  ::ecorecpp::serializer::serializer _ser(name);
  jobResultOutSerialized = strdup(_ser.serialize(jobResult).c_str());

  //OUT Parameter
  diet_string_set(diet_parameter(pb,3), jobResultOutSerialized, DIET_VOLATILE);

  char* ID2 = NULL;
  char* ID3 = NULL;
  std::string path = "/home/capo-chichi/tmp/test1.txt";
  std::string path2 = "/home/capo-chichi/tmp/test2.txt";

  dagda_init_container(diet_parameter(pb,4));

  dagda_put_file(strdup(path.c_str()), DIET_STICKY_RETURN, &ID2);
  dagda_put_file(strdup(path2.c_str()), DIET_STICKY_RETURN, &ID3);


  dagda_add_container_element((*diet_parameter(pb,4)).desc.id, ID2, 0);
  dagda_add_container_element((*diet_parameter(pb,4)).desc.id, ID3, 1);

  diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);

  return 0;
}


/* jobOutPutGetAllResult */
int
solve_jobOutPutGetAllResult(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  std::string listOfAlljobResults = "From server";

  cout << "Solve jobOutPutGetAllResult " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;

  string  errorInfo = ""/*"15#connectErrorMessage"*/;
  diet_string_set(diet_parameter(pb,2), strdup(listOfAlljobResults.c_str()), DIET_VOLATILE);

  char* ID2;
  char* ID3;
  std::string path = "/home/capo-chichi/tmp/test2.txt";
  std::string path2 = "/home/capo-chichi/tmp/test1.txt";

  dagda_init_container(diet_parameter(pb,3));

  //dagda_put_scalar(l3, DIET_LONGINT, DIET_PERSISTENT, &ID2);
  //dagda_put_scalar(l4, DIET_LONGINT, DIET_PERSISTENT, &ID3);
  dagda_put_file(strdup(path.c_str()), DIET_STICKY_RETURN, &ID2);
  dagda_put_file(strdup(path2.c_str()), DIET_STICKY_RETURN, &ID3);

  /*dagda_add_container_element((*diet_parameter(pb,3)).desc.id, ID2, 2);
  std::cout << "ADD1" << std::endl;
  dagda_add_container_element((*diet_parameter(pb,3)).desc.id, ID3, 3);
  std::cout << "ADD2" << std::endl;*/

  dagda_add_container_element((*diet_parameter(pb,3)).desc.id, ID2, 0);
  dagda_add_container_element((*diet_parameter(pb,3)).desc.id, ID3, 1);


  if (diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
    return 1;
  }
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
  profile = diet_profile_desc_alloc((SERVICES[0]+std::string(machineId)).c_str(), 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,6), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_submitJob)) return 1;

  /* cancelJob */
  profile = diet_profile_desc_alloc((SERVICES[1]+std::string(machineId)).c_str(), 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_cancelJob)) return 1;

  /* ListJob et getJobInfo */
  profile = diet_profile_desc_alloc((SERVICES[2]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_ListOfJobs)) return 1;

  /* getJobsProgression_ */
  profile = diet_profile_desc_alloc((SERVICES[3]+std::string(machineId)).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_getJobsProgression)) return 1;


  /* ListQueues */
  profile = diet_profile_desc_alloc((SERVICES[4]+std::string(machineId)).c_str(), 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_TMSMachineGetListOfQueues)) return 1;

  /* setMachineRefreshPeriod */
  profile = diet_profile_desc_alloc((SERVICES[5]+std::string(machineId)).c_str(), 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_TMSMachineRefreshPeriodSet)) return 1;

  /* getMachineRefreshPeriod */
  profile = diet_profile_desc_alloc((SERVICES[6]+std::string(machineId)).c_str(), 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_TMSMachineRefreshPeriodGet)) return 1;

  /* jobOutPutGetResult */
  profile = diet_profile_desc_alloc((SERVICES[7]+std::string(machineId)).c_str(), 2, 2, 5);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_CONTAINER, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,5), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_jobOutPutGetResult)) return 1;

  /* jobOutPutGetAllResult */
  profile = diet_profile_desc_alloc((SERVICES[8]+std::string(machineId)).c_str(), 1, 1, 4);
  diet_generic_desc_set(diet_param_desc(profile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,3), DIET_CONTAINER, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(profile,4), DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(profile, NULL, solve_jobOutPutGetAllResult)) return 1;

  diet_profile_desc_free(profile);
  diet_print_service_table();

  res = diet_SeD(argv[1], argc, argv);

  /* Not reached */
 return res;
}
