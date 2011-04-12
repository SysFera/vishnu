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
#include "VishnuException.hpp"
#include "utilVishnu.hpp"

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

////////////////////////////////////////////////////////////////////////////////////////////

int jobSubmit(TMS_Data::Job& job, char* jobScriptPath,
              const TMS_Data::SubmitOptions& options, 
              BatchServer* batchServer, char **envp) {
      return batchServer->submit(jobScriptPath, options, job, envp);
}

int jobCancel(TMS_Data::Job& job, BatchServer* batchServer) {
        return batchServer->cancel(job.getJobId().c_str());
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
  std::string action;
  char* jobSerializedPath = NULL;
  char* optionsPath = NULL;
  char* slaveJobFile = NULL;
  char* slaveErrorPath = NULL;
  char* jobScriptPath = NULL;

  if(argc < 2) {
    cerr << "Usage: " << argv[0] << " ACTION[SUBMIT|CANCEL] <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>  <SubmitOptionsSerializedPath>" << endl;    
    cerr << " <job_script_path>" << endl;
    exit(EXIT_FAILURE);
  }
  action = std::string(argv[1]);

  if(action.compare("SUBMIT")==0) {
    if(argc < 6) {
      cerr << "Usage: " << argv[0] << " <SUBMIT> <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath> <SubmitOptionsSerializedPath>" << endl;
      cerr << " <JobScriptPath>" << endl;
      exit(EXIT_FAILURE);
    }
    slaveJobFile = argv[4];
    optionsPath = argv[5];
    jobScriptPath = argv[6];
  }
  else if(action.compare("CANCEL")==0) {
    if(argc < 4) {
      cerr << "Usage: " << argv[0] << " <CANCEL> <JobSerializedPath>  <SlaveErrorPath>" << endl;
      exit(EXIT_FAILURE);
    }
  } else {
    cerr << "Unknown ACTION, it must be SUBMIT or CANCEL..." << endl;
    cerr << "Usage: " << argv[0] << " ACTION[SUBMIT|CANCEL] <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>  <SubmitOptionsSerializedPath>" << endl;
    exit(EXIT_FAILURE);
  }
 
  jobSerializedPath = argv[2];
  slaveErrorPath = argv[3];

  try {
   
    std::string jobSerialized = vishnu::get_file_content(jobSerializedPath);
    TMS_Data::Job_ptr job = NULL;
    if(!parseTMSEmfObject(std::string(jobSerialized), job)) {
      cout << "parseEmfObject returns NULL...." << endl;
      return 1;
    }

    cout << job << endl;
    TorqueServer* torqueServer = new TorqueServer();
    if(action.compare("SUBMIT")==0) {
      std::string options  = vishnu::get_file_content(optionsPath);
      TMS_Data::SubmitOptions_ptr submitOptions = NULL;
      if(!parseTMSEmfObject(std::string(options), submitOptions)) {
        cout << "parseEmfObject returns NULL...." << endl;
        return 1;
      }

      cout << submitOptions << endl;

      jobSubmit(*job, jobScriptPath, *submitOptions, torqueServer, envp);

      //To serialize the user object
      const char* name = "Job";
      ::ecorecpp::serializer::serializer _ser(name);
      std::string slaveJob = strdup(_ser.serialize(job).c_str());

      ::ecorecpp::serializer::serializer _ser2(name);
      std::string slaveOptions = strdup(_ser2.serialize(submitOptions).c_str());

      std::ofstream os_slaveJobFile(slaveJobFile);
      os_slaveJobFile << slaveJob;
      os_slaveJobFile.close();

      delete submitOptions;
    } else if(action.compare("CANCEL")==0) {

      jobCancel(*job, torqueServer);

    }
    delete job;
    delete torqueServer;
  } catch (VishnuException& ve) {
    std::string errorInfo =  ve.buildExceptionString();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
    std::cout << errorInfo << std::endl;
  } catch (std::exception& e) {
    std::string errorInfo = e.what();
    std::cout << errorInfo << std::endl;
    //os_error << errorInfo;
    //os_error.close();
  }

 
  return 0;
}

