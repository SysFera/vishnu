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


//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"
#include "DIET_client.h"
//#include "DIET_Dagda.h"

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
	string submitOptionsSerialized;
	string jobSerialized;
	string sessionKey;

	std::string errorInfo ="";
	std::string finishError ="";
	int mapperkey;
	std::string cmd = "";

	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, machineId);
	diet_string_get(pb,2, scriptContent);
	diet_string_get(pb,3, submitOptionsSerialized);
	diet_string_get(pb,4, jobSerialized);


	SessionServer sessionServer = SessionServer(sessionKey);

	try {
		//MAPPER CREATION
		Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
		mapperkey = mapper->code("vishnu_submit_job");
		mapper->code(machineId, mapperkey);
		mapper->code(submitOptionsSerialized, mapperkey);
		mapper->code(jobSerialized, mapperkey);
		cmd = mapper->finalize(mapperkey);

		TMS_Data::Job_ptr job = NULL;
		TMS_Data::SubmitOptions_ptr submitOptions = NULL;

		if(!vishnu::parseEmfObject(jobSerialized, job)) {
			throw SystemException(ERRCODE_INVDATA, "solve_submitJob: Job object is not well built");
		}

		if(!vishnu::parseEmfObject(submitOptionsSerialized, submitOptions)) {
			throw SystemException(ERRCODE_INVDATA, "solve_submitJob: SubmitOptions object is not well built");
		}

		JobServer jobServer(sessionServer, machineId, *job,
                                    ServerTMS::getInstance()->getBatchType(),
                                    ServerTMS::getInstance()->getBatchVersion());
		int vishnuId = ServerTMS::getInstance()->getVishnuId();
		std::string slaveDirectory = ServerTMS::getInstance()->getSlaveDirectory();
		jobServer.submitJob(scriptContent, *submitOptions, vishnuId, slaveDirectory, ServerTMS::getInstance()->getDefaultBatchOption());
		*job = jobServer.getData();

		::ecorecpp::serializer::serializer _ser;
		string updateJobSerialized = _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(job));

		diet_string_set(pb,5, updateJobSerialized.c_str());
		diet_string_set(pb,6);

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
		diet_string_set(pb,5);
		diet_string_set(pb,6, errorInfo.c_str());
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
	std::string jobSerialized;
	std::string finishError ="";
	int mapperkey;
	std::string cmd = "";
	std::string errorInfo ="";

	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, machineId);
	diet_string_get(pb,2, jobSerialized);

	SessionServer sessionServer = SessionServer(sessionKey);
	TMS_Data::Job_ptr job = NULL;

	try {
		//MAPPER CREATION
		Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
		mapperkey = mapper->code("vishnu_cancel_job");
		mapper->code(machineId, mapperkey);
		mapper->code(jobSerialized, mapperkey);
		cmd = mapper->finalize(mapperkey);

		if(!vishnu::parseEmfObject(jobSerialized, job)) {
			SystemException(ERRCODE_INVDATA, "solve_cancelJob: Job object is not well built");
		}

		JobServer jobServer(sessionServer, machineId, *job,
                                    ServerTMS::getInstance()->getBatchType(),
                                    ServerTMS::getInstance()->getBatchVersion());
		jobServer.cancelJob(ServerTMS::getInstance()->getSlaveDirectory());

		diet_string_set(pb,3, errorInfo.c_str());
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
		diet_string_set(pb,3, errorInfo.c_str());
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
	std::string errorInfo;
	std::string finishError ="";
	int mapperkey;
	std::string cmd = "";

	//IN Parameters
	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, machineId);
	diet_string_get(pb,2, jobSerialized);

	SessionServer sessionServer = SessionServer(sessionKey);

	try{
		//MAPPER CREATION
		Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
		mapperkey = mapper->code("vishnu_get_job_info");
		mapper->code(machineId, mapperkey);
		mapper->code(jobSerialized, mapperkey);
		cmd = mapper->finalize(mapperkey);

		TMS_Data::Job_ptr job = NULL;
		if(!parseEmfObject(jobSerialized, job)) {
			throw SystemException(ERRCODE_INVDATA, "solveJobOutPutGetResult: Job object is not well built");
		}

		JobServer jobServer(sessionServer, machineId, *job,
                                    ServerTMS::getInstance()->getBatchType(),
                                    ServerTMS::getInstance()->getBatchVersion());
		*job = jobServer.getJobInfo();

		::ecorecpp::serializer::serializer _ser;
		string updateJobSerialized = _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(job));

		//OUT Parameter
		diet_string_set(pb,3, updateJobSerialized.c_str());
		diet_string_set(pb,4);
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
		diet_string_set(pb,3);
		diet_string_set(pb,4, errorInfo.c_str());
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

		diet_string_set(pb,3, listQueuesSerialized.c_str());
		diet_string_set(pb,4, errorInfo.c_str());
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
		diet_string_set(pb,3);
		diet_string_set(pb,4, errorInfo.c_str());
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
		if(!parseEmfObject(jobResultSerialized, jobResult)) {
			throw SystemException(ERRCODE_INVDATA, "solveJobOutPutGetResult: jobResult object is not well built");
		}

		//Start dealing with output
		JobOutputServer jobOutputServer(sessionServer, machineId, *jobResult);
		TMS_Data::JobResult result = jobOutputServer.getJobOutput();
		string jobFiles =  vishnu::getResultFiles(result, false) ;
		string outputInfo = "/tmp/vishnu-"+result.getJobId()+"-outdescrXXXXXX"; // extension by convention

		vishnu::createTmpFile(const_cast<char*>(outputInfo.c_str()), jobFiles) ;

		diet_string_set(pb,4, outputInfo.c_str());

		sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
	} catch (VishnuException& e) {
		std::string finishError ="";
		try {
			sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
		} catch (VishnuException& fe) {
			finishError =  fe.what();
			finishError +="\n";
		}
		e.appendMsgComp(finishError);
		diet_string_set(pb,4, e.buildExceptionString().c_str());

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
	std::string errorInfo;
	int mapperkey;
	std::string cmd;
	std::string finishError ="";

	//IN Parameters
	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, machineId);
	diet_string_get(pb,2, optionValueSerialized);

	SessionServer sessionServer  = SessionServer(sessionKey);

	QueryParameters* options = NULL;
	List* list = NULL;

	try {

		//To parse the object serialized
		if(!parseEmfObject(optionValueSerialized, options)) {
			throw UMSVishnuException(ERRCODE_INVALID_PARAM);
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
		diet_string_set(pb,3, listSerialized.c_str());
		diet_string_set(pb,4);
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
		//Send error
		diet_string_set(pb,3, listSerialized.c_str());
		diet_string_set(pb,4, errorInfo.c_str());
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
	std::string moutDir;
	std::string jobsOutputSerialized;
	int mapperkey;
	std::string cmd;

	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, machineId);
	diet_string_get(pb,2, moutDir);

	SessionServer sessionServer = SessionServer(sessionKey);

	try {
		//MAPPER CREATION
		Mapper *mapper = MapperRegistry::getInstance()->getMapper(TMSMAPPERNAME);
		mapperkey = mapper->code("vishnu_get_completed_jobs_output");
		mapper->code(machineId, mapperkey);
		mapper->code(moutDir, mapperkey);
		cmd = mapper->finalize(mapperkey);

		JobOutputServer jobOutputServer(sessionServer, machineId);
		TMS_Data::ListJobResults_ptr completedJobsOutput = jobOutputServer.getCompletedJobsOutput();

		::ecorecpp::serializer::serializer _ser;
		jobsOutputSerialized =  _ser.serialize_str(completedJobsOutput);

		std::ostringstream ossFileName ;
		int nbResult = completedJobsOutput->getResults().size() ;
		for(size_t i = 0; i < nbResult; i++) {
			ostringstream missingFiles ; missingFiles.clear() ;
			ossFileName << vishnu::getResultFiles(*completedJobsOutput->getResults().get(i), true) ;
		}
		string outputInfo = "/tmp/vishnu-outdescrXXXXXX"; // extension by convention
		vishnu::createTmpFile(const_cast<char*>(outputInfo.c_str()), ossFileName.str()) ;

		diet_string_set(pb,3, outputInfo.c_str());
		diet_string_set(pb,4, jobsOutputSerialized.c_str());

		sessionServer.finish(cmd, TMS, vishnu::CMDSUCCESS);
	} catch (VishnuException& e) {
		std::string finishError ="";
		try {
			sessionServer.finish(cmd, TMS, vishnu::CMDFAILED);
		} catch (VishnuException& fe) {
			finishError =  fe.what();
			finishError +="\n";
		}
		e.appendMsgComp(finishError);
		diet_string_set(pb,3, e.buildExceptionString().c_str());
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
	std::string errorInfo;
	int mapperkey;
	std::string cmd;
	std::string finishError ="";

	//IN Parameters
	diet_string_get(pb,0, sessionKey);
	diet_string_get(pb,1, workSerialized);
	diet_string_get(pb,2, opSerialized);

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
			throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
		}
		//To parse the object serialized
		if(!parseEmfObject(opSerialized, workop)) {
			throw UMSVishnuException(ERRCODE_INVALID_PARAM, msgComp);
		}

		WorkServer workServer = WorkServer(work, sessionServer);
		workServer.add(ServerTMS::getInstance()->getVishnuId(), workop);

		//To serialize the user object
		::ecorecpp::serializer::serializer _ser;
		std::string workSerializedUpdate = _ser.serialize_str(work);

		//OUT Parameter
		diet_string_set(pb,3, workSerializedUpdate.c_str());
		diet_string_set(pb,4);
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
		diet_string_set(pb,3);
		diet_string_set(pb,4, errorInfo.c_str());
	}
	delete work;
	return 0;
}
