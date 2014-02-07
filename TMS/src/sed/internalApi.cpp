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
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>


//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"
#include "DIET_client.h"

#include "utilServer.hpp"
#include "BatchServer.hpp"
#include "ServerTMS.hpp"
#include "JobServer.hpp"
#include "WorkServer.hpp"
#include "ListJobServer.hpp"
#include "ListQueuesServer.hpp"
#include "ListProgressServer.hpp"
#include "JobOutputServer.hpp"
#include "WorkServer.hpp"

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

  std::string scriptContent;
  string machineId;
  string jsonEncodedOptions;
  string sessionKey;

  int mapperkey;
  std::string cmd = "";

  // get profile parameters
  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, machineId);
  diet_string_get(pb,2, scriptContent);
  diet_string_get(pb,3, jsonEncodedOptions);

  // reset the profile to send back result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_submit_job");
    mapper->code(machineId, mapperkey);
    mapper->code(jsonEncodedOptions, mapperkey);
    cmd = mapper->finalize(mapperkey);

    //FIXME: decode job and options
    JsonObject options(jsonEncodedOptions);

    ServerTMS* server = ServerTMS::getInstance();
    JobServer jobServer(sessionServer, machineId, server->getSedConfig());
    jobServer.setDebugLevel(server->getDebugLevel()); // Set the debug level

    int vishnuId = server->getVishnuId();
    jobServer.submitJob(scriptContent, options, vishnuId, server->getDefaultBatchOption());

    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, JsonObject::serialize(jobServer.getData()));

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS, std::string(jobServer.getData().getJobId()));

  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, ex.what());
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

  std::string sessionKey;
  std::string machineId;
  std::string optionSerialized;
  int mapperkey;
  std::string cmd = "";

  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, machineId);
  diet_string_get(pb,2, optionSerialized);

  // reset the profile to send back result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  TMS_Data::CancelOptions_ptr optionObjPtr = NULL;

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_cancel_job");
    mapper->code(machineId, mapperkey);
    mapper->code(optionSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    if(!vishnu::parseEmfObject(optionSerialized, optionObjPtr)) {
      SystemException(ERRCODE_INVDATA, "corrupted serialized options");
    }

    ServerTMS* server = ServerTMS::getInstance();
    TMS_Data::Job job;
    job.setJobId(optionObjPtr->getJobId());
    job.setUserId(optionObjPtr->getUser());
    JobServer jobServer(sessionServer, machineId, job, server->getSedConfig());
    jobServer.cancelJob();

    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, "");
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, ex.what());
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

  std::string sessionKey;
  std::string machineId;
  std::string jobSerialized;
  int mapperkey;
  std::string cmd = "";

  //IN Parameters
  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, machineId);
  diet_string_get(pb, 2, jobSerialized);

  // reset the profile to send back result
  diet_profile_reset(pb, 2);
  SessionServer sessionServer = SessionServer(sessionKey);

  try{
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_job_info");
    mapper->code(machineId, mapperkey);
    mapper->code(jobSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    JsonObject jobJson(jobSerialized);
    JobServer jobServer(sessionServer, machineId, jobJson.getJob(), ServerTMS::getInstance()->getSedConfig());

    jobSerialized = JsonObject::serialize(jobServer.getJobInfo());
    diet_string_set(pb,1, jobSerialized);
    diet_string_set(pb,0, "success");

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      e.appendMsgComp(fe.what());
    }
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, e.what());
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

  std::string sessionKey;
  std::string machineId;
  std::string option;
  std::string errorInfo ="";
  std::string listQueuesSerialized;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";

  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, machineId);
  diet_string_get(pb,2, option);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  TMS_Data::ListQueues_ptr listQueues = NULL;


  ListQueuesServer queryQueues(sessionServer, machineId,
                               ServerTMS::getInstance()->getBatchType(),
                               ServerTMS::getInstance()->getBatchVersion(),
                               option);

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);

    mapperkey = mapper->code("vishnu_list_queues");
    mapper->code(machineId, mapperkey);
    mapper->code(option, mapperkey);
    cmd = mapper->finalize(mapperkey);

    listQueues = queryQueues.list();

    ::ecorecpp::serializer::serializer _ser;
    listQueuesSerialized =  _ser.serialize_str(listQueues);


    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, listQueuesSerialized);
    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, ex.what());
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

  std::string sessionKey;
  std::string machineId;
  std::string jobResultSerialized;
  std::string moutDir;
  std::string cmd = "";

  //IN Parameters
  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, machineId);
  diet_string_get(pb,2, jobResultSerialized);
  diet_string_get(pb,3, moutDir);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    int mapperkey = mapper->code("vishnu_get_job_output");
    mapper->code(machineId, mapperkey);
    mapper->code(jobResultSerialized, mapperkey);
    mapper->code(moutDir, mapperkey);
    cmd = mapper->finalize(mapperkey);


    TMS_Data::JobResult_ptr jobResult = NULL;
    if (!parseEmfObject(jobResultSerialized, jobResult)) {
      throw SystemException(ERRCODE_INVDATA, "solveJobOutPutGetResult: jobResult object is not well built");
    }

    //Start dealing with output
    JobOutputServer jobOutputServer(sessionServer, machineId, *jobResult);
    TMS_Data::JobResult result = jobOutputServer.getJobOutput();
    string jobFiles =  vishnu::getResultFiles(result, false) ;
    string outputInfo = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-"+result.getJobId()+"-outdescr%%%%%%%").string(); // extension by convention
    vishnu::saveInFile(outputInfo, jobFiles);

    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, outputInfo);

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      e.appendMsgComp(fe.what());
    }

    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, e.what());
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

  std::string sessionKey;
  std::string machineId;
  std::string optionValueSerialized;
  std::string listSerialized = "";
  int mapperkey;
  std::string cmd;

  //IN Parameters
  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, machineId);
  diet_string_get(pb,2, optionValueSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer  = SessionServer(sessionKey);

  QueryParameters* options = NULL;
  List* list = NULL;

  try {

    //To parse the object serialized
    if(!parseEmfObject(optionValueSerialized, options)) {
      throw TMSVishnuException(ERRCODE_INVALID_PARAM);
    }
    QueryType query(options, sessionServer, machineId);

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code(query.getCommandName());
    mapper->code(machineId, mapperkey);
    mapper->code(optionValueSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    list = query.list();

    ::ecorecpp::serializer::serializer _ser;
    listSerialized =  _ser.serialize_str(list);

    //OUT Parameter
    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, listSerialized);

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& ex) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      ex.appendMsgComp(fe.what());
    }
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1,  ex.what());
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
  std::string sessionKey;
  std::string machineId;
  std::string serializedOptions;
  std::string jobListsSerialized;
  int mapperkey;
  std::string cmd;

  diet_string_get(pb, 0, sessionKey);
  diet_string_get(pb, 1, machineId);
  diet_string_get(pb, 2, serializedOptions);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_completed_jobs_output");
    mapper->code(machineId, mapperkey);
    mapper->code(serializedOptions, mapperkey);
    cmd = mapper->finalize(mapperkey);

    TMS_Data::JobOuputOptions_ptr options = NULL;
    if(!vishnu::parseEmfObject(serializedOptions, options)) {
      throw SystemException(ERRCODE_INVDATA, "the option object is not well built");
    }

    JobOutputServer jobOutputServer(sessionServer, machineId);
    TMS_Data::ListJobResults_ptr jobResults = jobOutputServer.getCompletedJobsOutput(*options);

    ::ecorecpp::serializer::serializer _ser;
    jobListsSerialized = _ser.serialize_str(jobResults);

    std::ostringstream ossFileName ;
    int nbResult = jobResults->getResults().size() ;
    for(size_t i = 0; i < nbResult; i++) {
      ostringstream missingFiles ; missingFiles.clear() ;
      ossFileName << vishnu::getResultFiles(*jobResults->getResults().get(i), true);
    }
    string outputInfo = bfs::unique_path(boost::filesystem::temp_directory_path().string()+"/vishnu-outdescr%%%%%%%").string();
    vishnu::saveInFile(outputInfo, ossFileName.str());

    JsonObject data;
    data.setProperty("infofile", outputInfo);
    data.setProperty("joblist", jobListsSerialized);

    // set result
    diet_string_set(pb, 0, "success");
    diet_string_set(pb, 1, data.encode());

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      e.appendMsgComp(fe.what());
    }
    // set result
    diet_string_set(pb, 0, "error");
    diet_string_set(pb, 1, e.what());
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
  std::string sessionKey;
  std::string workSerialized;
  std::string opSerialized;
  int mapperkey;
  std::string cmd;

  //IN Parameters
  diet_string_get(pb,0, sessionKey);
  diet_string_get(pb,1, workSerialized);
  diet_string_get(pb,2, opSerialized);

  // reset profile to handle result
  diet_profile_reset(pb, 2);

  SessionServer sessionServer = SessionServer(sessionKey);

  TMS_Data::AddWorkOptions_ptr workop = NULL;
  TMS_Data::Work_ptr work = NULL;

  try {
    std::string msgComp = "";

    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_add_work");
    mapper->code(workSerialized, mapperkey);
    mapper->code(opSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    //To parse the object serialized
    if(!parseEmfObject(workSerialized, work)) {
      throw TMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
    }
    //To parse the object serialized
    if(!parseEmfObject(opSerialized, workop)) {
      throw TMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
    }

    WorkServer workServer = WorkServer(work, sessionServer);
    workServer.add(ServerTMS::getInstance()->getVishnuId(), workop);

    //To serialize the user object
    ::ecorecpp::serializer::serializer _ser;
    std::string workSerializedUpdate = _ser.serialize_str(work);

    // set result
    diet_string_set(pb,0, "success");
    diet_string_set(pb,1, workSerializedUpdate);

    sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS, work->getWorkId());
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      e.appendMsgComp(fe.what());
    }

    // set result
    diet_string_set(pb,0, "error");
    diet_string_set(pb,1, e.what());
  }
  delete work;
  return 0;
}
