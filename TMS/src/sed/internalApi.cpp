/**
 * \file TMS/src/sed/internalApi.cpp
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
#include "DIET_client.h"
//#include "DIET_Dagda.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "ServerTMS.hpp"
#include "SSHJobExec.hpp"
#include "JobServer.hpp"
#include "ListJobServer.hpp"
#include "ListQueuesServer.hpp"
#include "ListProgressServer.hpp"
#include "JobOutputServer.hpp"

#include "internalApi.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespac

using namespace std;
using namespace vishnu;

/**
 * \brief Function to solve the jobSubmit service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveSubmitJob(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* submitOptionsSerialized  = NULL;
  char* jobSerialized = NULL;
  char* updateJobSerialized = NULL;
  char* script_content = NULL;
  std::string empty("");
  std::string errorInfo ="";
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &script_content, NULL);
  diet_string_get(diet_parameter(pb,3), &submitOptionsSerialized, NULL);
  diet_string_get(diet_parameter(pb,4), &jobSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_submit_job");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(submitOptionsSerialized), mapperkey);
    mapper->code(std::string(jobSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    TMS_Data::Job_ptr job = NULL;
    TMS_Data::SubmitOptions_ptr submitOptions = NULL;

    if(!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
      throw SystemException(ERRCODE_INVDATA, "solve_submitJob: Job object is not well built");
    }

    if(!vishnu::parseEmfObject(std::string(submitOptionsSerialized), submitOptions)) {
      throw SystemException(ERRCODE_INVDATA, "solve_submitJob: SubmitOptions object is not well built");
    }

    if(std::string(machineId).compare(AUTOMATIC_SUBMIT_JOB_KEYWORD)==0) {
      sessionServer.check(); //To check the sessionKey
      machineId = strdup((ServerTMS::getInstance()->getMachineId()).c_str());
      try {
        UserServer(sessionServer).getUserAccountLogin(machineId);
      } catch (VishnuException& e) {
        throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You have not a local account on any of the machines.");
      }
    } else {
      if(submitOptions->getCriterion()!=NULL) {
        throw UserException(ERRCODE_INVALID_PARAM, "Criterion option is used only if the machine identifier is equal"
            " to autom (this keyword is used to submit automatically a job)");
      }
    }

    JobServer jobServer(sessionServer, machineId, *job, ServerTMS::getInstance()->getBatchType());
    int vishnuId = ServerTMS::getInstance()->getVishnuId();
    std::string slaveDirectory = ServerTMS::getInstance()->getSlaveDirectory();
    jobServer.submitJob(script_content, *submitOptions, vishnuId, slaveDirectory);
    *job = jobServer.getData();

    ::ecorecpp::serializer::serializer _ser;
    updateJobSerialized = strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(job)).c_str());

    diet_string_set(diet_parameter(pb,5), updateJobSerialized, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS, std::string(jobServer.getData().getJobId()));
  } catch (VishnuException& e) {
      try {
        sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
      } catch (VishnuException& fe) {
        finishError =  fe.what();
        finishError +="\n";
      }
      e.appendMsgComp(finishError);
      errorInfo =  e.buildExceptionString();
      diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
 * \brief Function to solve the jobCancel service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveCancelJob(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* jobSerialized = NULL;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";
  std::string errorInfo ="";

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &jobSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  TMS_Data::Job_ptr job = NULL;

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_cancel_job");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(jobSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    if(!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
      SystemException(ERRCODE_INVDATA, "solve_cancelJob: Job object is not well built");
    }

    JobServer jobServer(sessionServer, machineId, *job, ServerTMS::getInstance()->getBatchType());
    jobServer.cancelJob(ServerTMS::getInstance()->getSlaveDirectory());

    diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
        sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
        finishError =  fe.what();
        finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,3), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

/**
 * \brief Function to solve the jobInfo service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveJobInfo(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* jobSerialized = NULL;
  char* updateJobSerialized = NULL;
  std::string empty = "";
  std::string errorInfo;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &jobSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try{
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_job_info");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(jobSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    TMS_Data::Job_ptr job = NULL;
    if(!parseEmfObject(std::string(jobSerialized), job)) {
      throw SystemException(ERRCODE_INVDATA, "solveJobOutPutGetResult: Job object is not well built");
    }

    JobServer jobServer(sessionServer, machineId, *job, ServerTMS::getInstance()->getBatchType());
    *job = jobServer.getJobInfo();

    ::ecorecpp::serializer::serializer _ser;
    updateJobSerialized = strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(job)).c_str());

    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), updateJobSerialized, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(empty.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
        finishError =  fe.what();
        finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
 * \brief Function to solve the getListOfQueues service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveListOfQueues(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* option = NULL;
  std::string empty("");
  std::string errorInfo ="";
  std::string listQueuesSerialized;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &option, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  TMS_Data::ListQueues_ptr listQueues = NULL;


  ListQueuesServer queryQueues(sessionServer, machineId, ServerTMS::getInstance()->getBatchType(), std::string(option));

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);

    mapperkey = mapper->code("vishnu_list_queues");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(option), mapperkey);
    cmd = mapper->finalize(mapperkey);

    listQueues = queryQueues.list();

    ::ecorecpp::serializer::serializer _ser;
    listQueuesSerialized =  _ser.serialize_str(listQueues);

    diet_string_set(diet_parameter(pb,3), strdup(listQueuesSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
        finishError =  fe.what();
        finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }

  return 0;
}

/**
 * \brief Function to solve the jobOutPutGetResult service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveJobOutPutGetResult(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* ID2 = NULL;
  char* ID3 = NULL;
  char* jobResultSerialized = NULL;
  char* moutDir = NULL;
  std::string empty = "";
  std::string errorInfo;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &jobResultSerialized, NULL);
  diet_string_get(diet_parameter(pb,3), &moutDir, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_job_output");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(jobResultSerialized), mapperkey);
    mapper->code(std::string(moutDir), mapperkey);
    cmd = mapper->finalize(mapperkey);


    TMS_Data::JobResult_ptr jobResult = NULL;
    if(!parseEmfObject(std::string(jobResultSerialized), jobResult)) {
      throw SystemException(ERRCODE_INVDATA, "solveJobOutPutGetResult: jobResult object is not well built");
    }

    JobOutputServer jobOutputServer(sessionServer, machineId, *jobResult);
    TMS_Data::JobResult result = jobOutputServer.getJobOutput();
    //OUT Parameter
    diet_string_set(diet_parameter(pb,4), strdup(empty.c_str()), DIET_VOLATILE);

    char* ID2 = NULL;
    char* ID3 = NULL;

    std::string outputPath = result.getOutputPath();
    std::string errorPath = result.getErrorPath();

    dagda_init_container(diet_parameter(pb,5));

    dagda_put_file(strdup(outputPath.c_str()), DIET_PERSISTENT_RETURN, &ID2);
    dagda_put_file(strdup(errorPath.c_str()), DIET_PERSISTENT_RETURN, &ID3);

    dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID2, 0);
    dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID3, 1);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);

  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
        finishError =  fe.what();
        finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);

    std::string outputPath = "error.txt";
    std::string errorPath = "error.txt";

    dagda_init_container(diet_parameter(pb,5));

    dagda_put_file(strdup(outputPath.c_str()), DIET_PERSISTENT_RETURN, &ID2);
    dagda_put_file(strdup(errorPath.c_str()), DIET_PERSISTENT_RETURN, &ID3);

    dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID2, 0);
    dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID3, 1);

  }
  return 0;
}

/**
 * \brief Function to solve the generic query service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
template <class QueryParameters, class List, class QueryType>
int
solveGenerique(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* optionValueSerialized = NULL;
  std::string listSerialized = "";
  std::string empty = "";
  std::string errorInfo;
  int mapperkey;
  std::string cmd;
  std::string finishError ="";

  //IN Parameters
  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &optionValueSerialized, NULL);

  SessionServer sessionServer  = SessionServer(std::string(sessionKey));

  QueryParameters* options = NULL;
  List* list = NULL;

  try {

    //To parse the object serialized
    if(!parseEmfObject(std::string(optionValueSerialized), options)) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM);
    }
    QueryType query(options, sessionServer, std::string (machineId));

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(optionValueSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    list = query.list();

    ::ecorecpp::serializer::serializer _ser;
    listSerialized =  _ser.serialize_str(list);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(listSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(empty.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
      try {
        sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
      } catch (VishnuException& fe) {
          finishError =  fe.what();
          finishError +="\n";
      }
      e.appendMsgComp(finishError);
      errorInfo =  e.buildExceptionString();
      //OUT Parameter
      diet_string_set(diet_parameter(pb,3), strdup(listSerialized.c_str()), DIET_VOLATILE);
      diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
  delete options;
  delete list;
  return 0;
}

/**
 * \brief Function to solve the getListOfJobs service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfJobs(diet_profile_t* pb) {
  return solveGenerique<TMS_Data::ListJobsOptions, TMS_Data::ListJobs, ListJobServer >(pb);
}

/**
 * \brief Function to solve the getJobsProgression service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfJobsProgression(diet_profile_t* pb) {
  return solveGenerique<TMS_Data::ProgressOptions, TMS_Data::ListProgression, ListProgressServer >(pb);
}

/**
 * \brief Function to solve the jobOutputGetCompletedJobs service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveJobOutPutGetCompletedJobs(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* machineId = NULL;
  char* moutDir = NULL;
  std::string errorInfo = "";
  std::string jobsOutputSerialized;
  std::string empty = "";
  int mapperkey;
  std::string cmd;
  std::string finishError ="";

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &machineId, NULL);
  diet_string_get(diet_parameter(pb,2), &moutDir, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_completed_jobs_output");
    mapper->code(std::string(machineId), mapperkey);
    mapper->code(std::string(moutDir), mapperkey);
    cmd = mapper->finalize(mapperkey);

    JobOutputServer jobOutputServer(sessionServer, machineId);
    TMS_Data::ListJobResults_ptr completedJobsOutput = jobOutputServer.getCompletedJobsOutput();

    ::ecorecpp::serializer::serializer _ser;
    jobsOutputSerialized =  _ser.serialize_str(completedJobsOutput);

    //OUT Parameter
    diet_string_set(diet_parameter(pb,3), strdup(jobsOutputSerialized.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);

    dagda_init_container(diet_parameter(pb,5));

    std::string outputPath;
    std::string errorPath;
    TMS_Data::JobResult_ptr jobResult = NULL;
    for(size_t i = 0; i < completedJobsOutput->getResults().size(); i++) {
      jobResult = completedJobsOutput->getResults().get(i);
      outputPath = jobResult->getOutputPath();
      errorPath = jobResult->getErrorPath();
      char* ID1 = NULL;
      char* ID2 = NULL;
      dagda_put_file(strdup(outputPath.c_str()), DIET_PERSISTENT_RETURN, &ID1);
      dagda_put_file(strdup(errorPath.c_str()), DIET_PERSISTENT_RETURN, &ID2);
      dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID1, 2*i);
      dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID2, 2*i+1);
    }
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
          finishError =  fe.what();
          finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);

    std::string outputPath = "error.txt";

    dagda_init_container(diet_parameter(pb,5));

    char* ID;
    dagda_put_file(strdup(outputPath.c_str()), DIET_PERSISTENT_RETURN, &ID);

    dagda_add_container_element((*diet_parameter(pb,5)).desc.id, ID, 0);
  }
  return 0;
}


/**
 * \brief Function to solve the service solveAddWork
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveAddWork(diet_profile_t* pb) {
	char *sessionKey = NULL;
	char *workSerialized = NULL;
	char *opSerialized = NULL;
	std::string empty("");
	std::string errorInfo;
	int mapperkey;
	std::string cmd;
	std::string finishError ="";

	//IN Parameters
	diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
	diet_string_get(diet_parameter(pb,1), &workSerialized, NULL);
	diet_string_get(diet_parameter(pb,2), &opSerialized, NULL);

	SessionServer sessionServer = SessionServer(std::string(sessionKey));

	TMS_Data::AddWorkOptions_ptr workop = NULL;
	TMS_Data::Work_ptr work = NULL;

	try {
		std::string msgComp = "";

		//MAPPER CREATION
		Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
		mapperkey = mapper->code("vishnu_add_work");
		mapper->code(std::string(workSerialized), mapperkey);
		mapper->code(std::string(opSerialized), mapperkey);
		cmd = mapper->finalize(mapperkey);

		//To parse the object serialized
		if(!parseEmfObject(std::string(workSerialized), work)) {
			throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
		}
		//To parse the object serialized
		if(!parseEmfObject(std::string(opSerialized), workop)) {
			throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
		}

		WorkServer workServer = WorkServer(work, sessionServer);
		workServer.add(ServerTMS::getInstance()->getVishnuId(), workop);

		//To serialize the user object
		::ecorecpp::serializer::serializer _ser;
		std::string workSerializedUpdate = _ser.serialize_str(work);

		//OUT Parameter
		diet_string_set(diet_parameter(pb,3), strdup(workSerializedUpdate.c_str()), DIET_VOLATILE);
		diet_string_set(diet_parameter(pb,4), strdup(empty.c_str()), DIET_VOLATILE);
		//To save the connection
		sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS, work->getWorkId());
	} catch (VishnuException& e) {
		try {
			sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
		} catch (VishnuException& fe) {
			finishError =  fe.what();
			finishError +="\n";
		}
		e.appendMsgComp(finishError);
		errorInfo =  e.buildExceptionString();
		//OUT Parameter
		diet_string_set(diet_parameter(pb,3), strdup(empty.c_str()), DIET_VOLATILE);
		diet_string_set(diet_parameter(pb,4), strdup(errorInfo.c_str()), DIET_VOLATILE);
	}
	delete work;
	return 0;
}
