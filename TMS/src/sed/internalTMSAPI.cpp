/**
 * \file internalTMSAPI.cpp 
 * \brief This file contains the VISHNU internal TMS API function.
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

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "TMS_Data.hpp"
#include "emfTMSUtils.hpp"

#include "DIET_server.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "TorqueServer.hpp"
#include "ServerTMS.hpp"
#include "SSHJobExec.hpp"
#include "JobServer.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac

using namespace std;
using namespace vishnu;

/*
 * SOLVE FUNCTIONS
 */
/* submitJob */
int
solve_submitJob(diet_profile_t* pb)
{

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* submitOptionsSerialized  = NULL;
  char* jobSerialized = NULL;
  char* updateJobSerialized = NULL;
  char* script_content = NULL;
  std::string empty("");
  std::string errorInfo ="";

  cout << "Solve submitJob " << endl;

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  cout << "************sessionKey=" << sessionKey << " ..." << endl;
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  cout << "************machineId=" << machineId << " ..." << endl;
  diet_string_get(diet_parameter(pb,2), &script_content, NULL);
  cout << "************script_content=" << script_content << " ..." << endl;
  diet_string_get(diet_parameter(pb,3), &submitOptionsSerialized, NULL);
  cout << "************options=" << submitOptionsSerialized << " ..." << endl;
  diet_string_get(diet_parameter(pb,4), &jobSerialized, NULL);
  cout << "************job=" << jobSerialized << " ..." << endl;

  std::cout << "The machine identifier is: " << ServerTMS::getMachineId() << std::endl;
  std::cout << "The batch identifier is: " << ServerTMS::getBatchType() << std::endl;

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  TMS_Data::Job_ptr job = NULL;
  TMS_Data::SubmitOptions_ptr submitOptions = NULL;

  try {

    if(!vishnu::parseTMSEmfObject(std::string(jobSerialized), job)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "solve_submitJob: Job object is not well built");  
    }

    if(!vishnu::parseTMSEmfObject(std::string(submitOptionsSerialized), submitOptions)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "solve_submitJob: SubmitOptions object is not well built"); 
    }

    JobServer jobServer(sessionServer, machineId, *job, ServerTMS::getBatchType());
    jobServer.submitJob(script_content, *submitOptions);
    *job = jobServer.getData();

    const char* name = "solve_submitJob";
    ::ecorecpp::serializer::serializer _ser(name);
    updateJobSerialized = strdup(_ser.serialize(const_cast<TMS_Data::Job_ptr>(job)).c_str());

    diet_string_set(diet_parameter(pb,5), updateJobSerialized, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
  } catch (VishnuException& e) {
    errorInfo =  e.buildExceptionString();
    std::cout << "errorInfo=" << errorInfo << std::endl;
    diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
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

  std::cout << "Cancel: The machine identifier is: " << ServerTMS::getMachineId() << std::endl;
  std::cout << "Cancel: The batch identifier is: " << ServerTMS::getBatchType() << std::endl;

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  TMS_Data::Job_ptr job = NULL;
  TMS_Data::SubmitOptions_ptr submitOptions = NULL;

  try {

    if(!vishnu::parseTMSEmfObject(std::string(jobSerialized), job)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "solve_cancelJob: Job object is not well built");
    }

    JobServer jobServer(sessionServer, machineId, *job, ServerTMS::getBatchType());
    jobServer.cancelJob();

    if(diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE)) {
      cerr << "diet_string_set error" << endl;
      return 1;
    }
  } catch (VishnuException& e) {
    errorInfo =  e.buildExceptionString();
    std::cout << "errorInfo=" << errorInfo << std::endl;
    diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  cout << " done" << endl;
  return 0;
}
