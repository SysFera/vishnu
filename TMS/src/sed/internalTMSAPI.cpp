#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"
#include "emfTMSUtils.hpp"

#include "DIET_server.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "TorqueServer.hpp"
#include "TMSServer.hpp"
#include "SSHJobExec.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac

using namespace std;
using namespace vishnu;

//////////////////////////////SONT a enlever c'est juste pour les test//////////////////////////
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

std::string get_file_content(const std::string& filePath) {

  bfs::path file (filePath);
  // Check existence of the file
  if ((false==bfs::exists(file)) || (true==bfs::is_empty(file)) ){
    cerr << "can not read the file: " + filePath << endl;;
    exit(EXIT_FAILURE);
  }

  bfs::ifstream ifs (file);

  // Read the whole file into string
  std::stringstream ss;
  ss << ifs.rdbuf();

  return ss.str();

}

////////////////////////////////////////////////////////////////////////////

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
  char* script_content = NULL;
  std::string errorInfo ="";
  std::string scriptPath = std::string(getenv("HOME"))+"/submit.pbs";

  cout << "Solve submitJob " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &script_content, NULL);
  cout << "************script_content=" << script_content << " ..." << endl;
  diet_string_get(diet_parameter(pb,3), &options, NULL);
  cout << "************options=" << options << " ..." << endl;
  diet_string_get(diet_parameter(pb,4), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  std::cout << "The machine identifier is: " << TMSServer::getMachineId() << std::endl;
  std::cout << "The batch identifier is: " << TMSServer::getBatchType() << std::endl;

  std::ofstream ofile(scriptPath.c_str());
  cout << "**************content of script *********" << endl;
  cout << script_content << endl;
  cout << "******************************************" << endl;
  ofile << script_content;
  ofile.close();

  SSHJobExec sshJobExec(const_cast<char*>(scriptPath.c_str()), jobSerialized, options, TMSServer::getBatchType());
  sshJobExec.sshexec("SUBMIT");
  updateJobSerialized = strdup(sshJobExec.getJobSerialized().c_str()); 
  errorInfo = sshJobExec.getErrorInfo();
  std::cout << "errorInfo=" << errorInfo << std::endl;
  if(diet_string_set(diet_parameter(pb,5), updateJobSerialized, DIET_VOLATILE)) {
    cerr << "diet_string_set error" << endl;
    return 1;
  }

  if(diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
    cerr << "diet_string_set error" << endl;
    return 1;
  }

  cout << " done" << endl;
 return 0;
}

/* cancelJob */
int
solve_cancelJob(diet_profile_t* pb)
{

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* jobSerialized = NULL;
  std::string errorInfo ="";

  cout << "Solve cancel " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  std::cout << "Cancel: The machine identifier is: " << TMSServer::getMachineId() << std::endl;
  std::cout << "Cancel: The batch identifier is: " << TMSServer::getBatchType() << std::endl;
  
  SSHJobExec sshJobExec(NULL, jobSerialized, "", TMSServer::getBatchType());
  sshJobExec.sshexec("CANCEL");
  errorInfo = sshJobExec.getErrorInfo();
  std::cout << "errorInfo=" << errorInfo << std::endl;


  if(diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
    cerr << "diet_string_set error" << endl;
    return 1;
  }

  cout << " done" << endl;
 return 0;
}
