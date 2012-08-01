/**
 * \file api_tms.cpp
 * \brief This file contains the VISHNU api functions for TMS package.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date March 2011
 */

#include <vector>
#include "api_tms.hpp"
#include "utilVishnu.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

//UMS include
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"
#include "LocalAccountProxy.hpp"
#include "ConfigurationProxy.hpp"
#include "OptionValueProxy.hpp"
#include "QueryProxy.hpp"
#include "UtilsProxy.hpp"
#include "utilVishnu.hpp"

//TMS include
#include "JobProxy.hpp"
#include "JobOutputProxy.hpp"
#include "WorkProxy.hpp"

/**
 * \brief The submitJob function submits job on a machine through a script pointed by scriptFilePath.
 * \param sessionKey : The session key
 * \param machineId : Is the id of the machine on which the job must be submitted
 * \param scriptFilePath : The path to the file containing the characteristics (job command, and batch scheduler directive required or optional) of the job to submit.
 * \param jobInfo : The  Job object containing the output information (ex: jobId and jobPath) of the job to submit
 * \param options : Is an instance of the class SubmitOptions. Each option is associated to a set operation (e.g: setNbCpu(...)) in the class.  If no set operation is not called on the instance object  options, the job is submitted with the options defined in the scriptFilePath. Otherewise the job is submitted with the optionnal values set by the options object and optionnal values defined in the scriptFilePath, but optionnal values set by SubmitOptions object take precedence over those in scriptFilePath. With in the object options or within the scriptFilePath, the last occurance of an optionnal value takes precedence over earlier occurance.
 * \return int : an error code
 */
int
vishnu::submitJob(const std::string& sessionKey,
		const std::string& machineId,
		const std::string& scriptFilePath,
		Job& jobInfo,
		const SubmitOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");
	//To check options value nbNodesAndCpuPerNode
	checkJobNbNodesAndNbCpuPerNode(options.getNbNodesAndCpuPerNode());

	SessionProxy sessionProxy(sessionKey);
	boost::filesystem::path completePath(scriptFilePath);
	std::string scriptFileCompletePath = (boost::filesystem::path(boost::filesystem::system_complete(completePath))).string();
	jobInfo.setJobPath(scriptFileCompletePath);

	std::string scriptContent = vishnu::get_file_content(scriptFilePath);

	JobProxy jobProxy(sessionProxy , machineId , jobInfo);

	ListStrings fileParamsVec;
	std::string fileParamsStr = options.getFileParams() ;
	boost::trim(fileParamsStr) ; //TODO BUG when empty list
	boost::split(fileParamsVec, fileParamsStr, boost::is_any_of(" "), boost::token_compress_on) ;

	int ret = jobProxy.submitJob(scriptContent, options);
	jobInfo = jobProxy.getData();

	return ret;
}

/**
 * \brief The cancelJob function cancels a job from its id
 * \param sessionKey : The session key
 * \param machineId : The id of the machine
 * \param jobId : The Id of the job
 * \param infoMsg : The information message
 * \return int : an error code
 */
int
vishnu::cancelJob(const std::string& sessionKey,
		const std::string& machineId,
		const std::string& jobId)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");
	checkEmptyString(jobId, "The job id");

	SessionProxy sessionProxy(sessionKey);

	TMS_Data::Job job;
	job.setJobId(jobId);

	return JobProxy(sessionProxy,
			machineId,
			job).cancelJob();

}


/**
 * \brief The listJobs function gets a list of all submitted jobs
bInfo function gets information on a job from its id
 * \param sessionKey : The session key
 * \param machineId : The id of the machine
 * \param jobId : The id of the job
 * \param jobInfos : The resulting information on the job
 * \return int : an error code
 */
int
vishnu::getJobInfo(const std::string& sessionKey,
		const std::string& machineId,
		const std::string& jobId,
		Job& jobInfo)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");
	checkEmptyString(jobId, "The job id");

	SessionProxy sessionProxy(sessionKey);
	jobInfo.setJobId(jobId);

	JobProxy jobProxy(sessionProxy,
			machineId,
			jobInfo);

	jobInfo = jobProxy.getJobInfo();

	return 0;

}

/**
 * \brief The listJobs function gets a list of all submitted jobs
 * \param sessionKey : The session key
 * \param machineId : The id of the machine
 * \param listOfJobs : The constructed object list of jobs
 * \param options : Additional options for jobs listing
 * \return int : an error code
 */
int
vishnu::listJobs(const std::string& sessionKey,
		const std::string& machineId,
		ListJobs& listOfJobs,
		const ListJobsOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");

	std::string serviceName = "getListOfJobs_";
	serviceName.append(machineId);

	SessionProxy sessionProxy(sessionKey);

	//To check the job status options
	checkJobStatus(options.getStatus());
	//To check the job priority options
	checkJobPriority(options.getPriority());

	QueryProxy<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>
	query(options, sessionProxy, serviceName, machineId);

	TMS_Data::ListJobs* listJobs_ptr = query.list();

	if(listJobs_ptr != NULL) {
		TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
		for(unsigned int i = 0; i < listJobs_ptr->getJobs().size(); i++) {
			TMS_Data::Job_ptr job = ecoreFactory->createJob();
			//To copy the content and not the pointer
			*job = *listJobs_ptr->getJobs().get(i);
			listOfJobs.getJobs().push_back(job);
		}
		listOfJobs.setNbJobs(listJobs_ptr->getJobs().size());
		listOfJobs.setNbRunningJobs(listJobs_ptr->getNbRunningJobs());
		listOfJobs.setNbWaitingJobs(listJobs_ptr->getNbWaitingJobs());
		delete listJobs_ptr;
	}
	return 0;
}

/**
 * \brief The getJobProgress function gets the progression status of jobs
 * \param sessionKey : The session key
 * \param machineId : Is the id of the machine to get the jobs progression.
 * \param listOfProgress : Is the object containing jobs progression information
 * \param options : Is an object containing the available options jobs for progression .
 * \return int : an error code
 */
int
vishnu::getJobProgress(const std::string& sessionKey,
		const std::string& machineId,
		ListProgression& listOfProgress,
		const ProgressOptions& options)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");

	std::string serviceName = "getJobsProgression_";
	serviceName.append(machineId);

	SessionProxy sessionProxy(sessionKey);

	QueryProxy<TMS_Data::ProgressOptions, TMS_Data::ListProgression>
	query(options, sessionProxy, serviceName, machineId);

	TMS_Data::ListProgression* listProgression_ptr = query.list();

	if(listProgression_ptr != NULL) {
		TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
		for(unsigned int i = 0; i < listProgression_ptr->getProgress().size(); i++) {
			TMS_Data::Progression_ptr progression = ecoreFactory->createProgression();
			//To copy the content and not the pointer
			*progression = *listProgression_ptr->getProgress().get(i);
			listOfProgress.getProgress().push_back(progression);
		}
		listOfProgress.setNbJobs(listProgression_ptr->getProgress().size());
		delete listProgression_ptr;
	}
	return 0;
}

/**
 * \brief The listQueues function gets queues information
 * \param sessionKey : The session key
 * \param machineId : The id of the machine
 * \param listofQueues : The list of queues
 * \param queueName The option value, if it is given, listQueues gives information only of this queue
 * \return int : an error code
 */
int
vishnu::listQueues(const std::string& sessionKey,
		const std::string& machineId,
		ListQueues& listofQueues,
		const std::string& queueName)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");

	std::string serviceName = "getListOfQueues_";
	serviceName.append(machineId);

	SessionProxy sessionProxy(sessionKey);

	QueryProxy<std::string, TMS_Data::ListQueues>
	query(queueName, sessionProxy, serviceName, machineId);

	TMS_Data::ListQueues* listQueues_ptr = query.list();

	if(listQueues_ptr != NULL) {
		TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
		for(unsigned int i = 0; i < listQueues_ptr->getQueues().size(); i++) {
			TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
			//To copy the content and not the pointer
			*queue = *listQueues_ptr->getQueues().get(i);
			listofQueues.getQueues().push_back(queue);
		}
		listofQueues.setNbQueues(listQueues_ptr->getQueues().size());
		delete listQueues_ptr;
	}
	return 0;

}

/**
 * \brief The getJobOutput function gets outputPath and errorPath of a job from its id
 * \param sessionKey : The session key
 * \param machineId : The Id of the machine
 * \param jobId : The Id of the job
 * \param outputInfo : The  Job object  containing the job output information (ex: outputPath and errorPath) of the job to submit
 * \param outDir : The output directory where the files will be stored (default is current directory)
 * \return int : an error code
 */
int
vishnu::getJobOutput(const std::string& sessionKey,
		const std::string& machineId,
		const std::string& jobId,
		JobResult& outputInfo,
		const std::string& outDir)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");

	if((outDir.size()!=0)&&(!boost::filesystem::exists(outDir))) {
		throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The directory "+outDir+" does not exist");
	}

	SessionProxy sessionProxy(sessionKey);
	JobOutputProxy jobOutputProxy(sessionProxy, machineId, outDir);

	outputInfo = jobOutputProxy.getJobOutPut(jobId);

	return 0;
}

/**
 * \brief The getCompletedJobsOutput() function gets standard output and error output files
 * of completed jobs (applies only once for each job)
 * \param sessionKey : The session key
 * \param machineId : The id of the machine
 * \param listOfResults : Is the list of jobs results
 * \param outDir : The output directory where the files will be stored (default is current directory)
 * \return int : an error code
 */
int
vishnu::getCompletedJobsOutput(const std::string& sessionKey,
		const std::string& machineId,
		ListJobResults& listOfResults,
		const std::string& outDir)
throw (UMSVishnuException, TMSVishnuException, UserException, SystemException) {

	checkEmptyString(sessionKey, "The session key");
	checkEmptyString(machineId, "The machine id");

	if((outDir.size()!=0)&&(!boost::filesystem::exists(outDir))) {
		throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The directory "+outDir+" does not exist");
	}

	SessionProxy sessionProxy(sessionKey);
	JobOutputProxy jobOutputProxy(sessionProxy, machineId, outDir);

	TMS_Data::ListJobResults_ptr listJobResults_ptr = jobOutputProxy.getCompletedJobsOutput();

	if(listJobResults_ptr != NULL) {
		TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
		for(unsigned int i = 0; i < listJobResults_ptr->getResults().size(); i++) {
			TMS_Data::JobResult_ptr jobResult = ecoreFactory->createJobResult();
			//To copy the content and not the pointer
			*jobResult = *listJobResults_ptr->getResults().get(i);
			listOfResults.getResults().push_back(jobResult);
		}
		listOfResults.setNbJobs(listJobResults_ptr->getNbJobs());
		delete listJobResults_ptr;
	}
	return 0;
}

int
vishnu::addWork(const std::string& sessionKey,
        Work& newWork)
  throw (UMSVishnuException, TMSVishnuException, UserException, SystemException){
  checkEmptyString(sessionKey, "The session key");
  SessionProxy sessionProxy(sessionKey);
  WorkProxy workProxy(sessionProxy);
  int res = workProxy.add(newWork);
  return res;
}

