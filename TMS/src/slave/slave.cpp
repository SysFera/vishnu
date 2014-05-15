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
    std::string msg = "Slave: Invalid batch. Valid batch are: TORQUE, LOADLEVLER, SLURM, "
                      "LSF, SGE, PBSPRO, DELTACLOUD, OPENNEBULA, POSIX\n";
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
    TMS_Data::Job jobInfo = jsonJob.getJob();

    if (action == "SUBMIT") {
      JsonObject jsonOptions(vishnu::get_file_content(optionsPath));
      if (! jobInfo.getOutputDir().empty()) {
        bool isWorkingDir = (batchType == DELTACLOUD)? true : false;
        vishnu::createDir(jobInfo.getOutputDir(), isWorkingDir); // Create the output directory
      }

      // create output dir if needed
      if (! jobInfo.getOutputDir().empty()) {
        vishnu::createDir(jobInfo.getOutputDir());
      }

      //Submits the job
      TMS_Data::ListJobs jobSteps;
      if (batchServer->submit(vishnu::copyFileToUserHome(jobScriptPath), jsonOptions.getSubmitOptions(), jobSteps) != 0) {
        throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "slave: the submission failed");
      }

      // store the serialized result
      vishnu::saveInFile(slaveJobFile, vishnu::emfSerializer<TMS_Data::ListJobs>(&jobSteps));
    } else if (action == "CANCEL") {
      switch (batchType) {
      case DELTACLOUD:
      case OPENNEBULA:
        batchServer->cancel(jobInfo.getVmId());
        break;
      default:
        batchServer->cancel(jobInfo.getBatchJobId());
        break;
      }
    }
  } catch (VishnuException& ve) {
    vishnu::saveInFile(slaveErrorPath, ve.buildExceptionString());
    ret = EXIT_FAILURE;
  } catch (std::exception& e) {
    vishnu::saveInFile(slaveErrorPath, e.what());
    ret = EXIT_FAILURE;
  }
  delete batchServer;
  return ret;
}
