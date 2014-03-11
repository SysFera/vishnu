/**
 * \file slave.cpp
 * \brief This file contains the VISHNU TMS slave main function.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */


#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"
#include "DIET_client.h"
#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "VishnuException.hpp"
#include "BatchFactory.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"
#include "tmsUtils.hpp"


namespace bfs=boost::filesystem; // an alias for boost filesystem namespac
using namespace std;
using namespace vishnu;


/**
 * \brief To show how to use the slave
 * \param cmd The name of the program
 * \return Always 1
 */
void usage(char* cmd) {
  cerr << "Usage: " << cmd << " COMMAND_TYPE[SUBMIT] <BatchType> <BatchVersion>"
       << " <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>"
       << " <SubmitOptionsSerializedPath> <job_script_path>\n"
       << "\t\t\t\t\t" << " or\n"
       << "Usage: " << cmd << " COMMAND_TYPE[CANCEL] <BatchType> <BatchVersion>"
       << " <JobSerializedPath> <SlaveErrorPath>\n";
  exit(EXIT_FAILURE);
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int
main(int argc, char* argv[]) {

  int ret = EXIT_SUCCESS;
  std::string action;
  char* jobSerializedPath = NULL;
  char* optionsPath = NULL;
  char* slaveJobFile = NULL;
  char* slaveErrorPath = NULL;
  char* jobScriptPath = NULL;
  BatchType batchType;
  std::string batchVersion;

  if(argc < 6) { // Too few arguments
    usage(argv[0]);
  }
  action = std::string(argv[1]);
  batchType = vishnu::convertToBatchType(argv[2]);   //Get batchType
  batchVersion = argv[3];
  jobSerializedPath = argv[4];
  slaveErrorPath = argv[5];

  if (action == "SUBMIT") {
    if(argc < 9) {       // Too few arguments
      usage(argv[0]);
    }
    slaveJobFile = argv[6];
    optionsPath = argv[7];
    jobScriptPath = argv[8];
  }

  if(batchType == UNDEFINED) {
    std::string msg = "Slave: Invalid batch. Batch type must be TORQUE, LOADLEVLER, SLURM, LSF, SGE, PBSPRO, DELTACLOUD or POSIX\n";
    std::cerr << msg;
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, msg);
  }

  BatchServer* batchServer;
  try {
    //To create batchServer Factory
    BatchFactory factory;
    batchServer = factory.getBatchServerInstance(batchType, batchVersion);
    if (! batchServer) {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "slave: getBatchServerInstance return NULL instance");
    }

    JsonObject jsonJob(vishnu::get_file_content(jobSerializedPath));
    TMS_Data::Job defaultJobInfo = jsonJob.getJob();

    if (action == "SUBMIT") {
      JsonObject jsonOptions(vishnu::get_file_content(optionsPath));
      if (! defaultJobInfo.getOutputDir().empty()) {
        bool isWorkingDir = (batchType == DELTACLOUD)? true : false;
        vishnu::createDir(defaultJobInfo.getOutputDir(), isWorkingDir); // Create the output directory
      }

      //Submits the job
      std::vector<TMS_Data::Job> jobSteps;
      if (batchServer->submit(jobScriptPath, jsonOptions.getSubmitOptions(), jobSteps) != 0) {
        throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "slave: the submission failed");
      }

      // serialize the result
      JsonObject jobStepsJson;
      jobStepsJson.setProperty("nbsteps", jobSteps.size());
      for (int step = 0; step < jobSteps.size(); ++step) {
        jobStepsJson.setProperty(boost::str(boost::format("step@%1%") % step), JsonObject::serialize(jobSteps[step]));
      }
      std::ofstream os_slaveJobFile(slaveJobFile);
      os_slaveJobFile << jobStepsJson.encode();
      os_slaveJobFile.close();
    } else if (action == "CANCEL") {
      if (batchType == DELTACLOUD) {
        std::string jobdDescr = boost::str(boost::format("%1%@%2%") % defaultJobInfo.getJobId() % defaultJobInfo.getVmId());
        batchServer->cancel(jobdDescr);
      } else {
        batchServer->cancel(defaultJobInfo.getBatchJobId());
      }
    }
  } catch (VishnuException& ve) {
    std::string errorInfo =  ve.buildExceptionString();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
    ret = EXIT_FAILURE;
  } catch (std::exception& e) {
    std::string errorInfo = e.what();
    std::ofstream os_error(slaveErrorPath);
    os_error << errorInfo;
    os_error.close();
    ret = EXIT_FAILURE;
  }
  delete batchServer;
  return ret;
}
