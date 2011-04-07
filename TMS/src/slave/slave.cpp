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

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac
using namespace std;
using namespace vishnu;

/////////////////////////////////////////ATTENTION A METRE dans le fichier de cli/////////////////////
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
  std::string jobScriptPath = job->getJobPath();
  std::string errorInfo = job->getJobDescription();

  os << "==============Job content ================" << endl;
  if(jobId.size()!=0) os << setw(25) << right << "jobId: " << jobId  << endl;
  if(jobScriptPath.size()!=0) os << setw(25) << right << "jobScriptPath: " << jobScriptPath  << endl;
  if(errorInfo.size()!=0) os << setw(25) << right << "errorInfos: " << errorInfo << endl;
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
////////////////////////////////////////////////////////////////////////////////////////////

int jobSubmit(TMS_Data::Job& job,
              const TMS_Data::SubmitOptions& options, 
              BatchServer* batchServer) {
      return batchServer->submit(job.getJobPath().c_str(), options, job);
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

  if (argc < 5) {
    cerr << "Usage: " << argv[0] << " <JobSerializedPath> <Options> <slaveJobFile> <slaveOptionsPath>" << endl;
    exit(EXIT_FAILURE);
  }
 
  char* jobSerializedPath = argv[1];
  char* optionsPath = argv[2];
  char* slaveJobFile = argv[3];
  char* slaveOptionsPath = argv[4];

  std::string jobSerialized = get_file_content(jobSerializedPath);
  TMS_Data::Job_ptr job = NULL;
  if(!parseTMSEmfObject(std::string(jobSerialized), job)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }
 
  cout << job << endl;

  std::string options  = get_file_content(optionsPath);
  TMS_Data::SubmitOptions_ptr submitOptions = NULL;
  if(!parseTMSEmfObject(std::string(options), submitOptions)) {
     cout << "parseEmfObject returns NULL...." << endl;
     return 1;
  }

  cout << submitOptions << endl;
 
  TorqueServer* torqueServer = new TorqueServer();
  int res = jobSubmit(*job, *submitOptions, torqueServer);
  cout << "After Job Submission..." << endl;
  cout << job << endl;
  if(res) exit(EXIT_FAILURE);// temporaire

  //To serialize the user object
  const char* name = "Job";
  ::ecorecpp::serializer::serializer _ser(name);
  std::string slaveJob = strdup(_ser.serialize(job).c_str());

  ::ecorecpp::serializer::serializer _ser2(name);
  std::string slaveOptions = strdup(_ser2.serialize(submitOptions).c_str());

  std::ofstream ofile1(slaveJobFile);
  ofile1 << slaveJob;
  ofile1.close();

  std::ofstream ofile2(slaveOptionsPath);
  ofile2 << slaveOptions;
  ofile2.close();

  return 0;
}

