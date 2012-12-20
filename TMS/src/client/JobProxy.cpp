
#include <iostream>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilClient.hpp"
#include "api_fms.hpp"
#include "api_ums.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/filesystem.hpp>
#include "tmsClientUtils.hpp"

using namespace std;
namespace bfs=boost::filesystem;
typedef vector<string> ListStrings ;
/**
 * \param session The object which encapsulates the session information
 * \param machineId The id of the machine
 * \param job The job data structure
 * \brief Constructor
 */
JobProxy::JobProxy(const SessionProxy& session,
		const std::string & machineId,
		TMS_Data::Job& job)
:msessionProxy(session), mmachineId(machineId), mjob(job) {

}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */
int
JobProxy::submitJob(const std::string& scriptContent,
		const TMS_Data::SubmitOptions& options) {

	string sessionKey = msessionProxy.getSessionKey().c_str();
	TMS_Data::SubmitOptions& options_ = const_cast<TMS_Data::SubmitOptions&>(options) ;

	// first check if it's an automatic submission
	// if yes, select a machine according to the load criterion
	if(mmachineId.compare(AUTOMATIC_SUBMIT_JOB_KEYWORD)==0) {
		mmachineId = findMachine(sessionKey, options_.getCriterion());
	}

	// now create and initialize the service profile
	string serviceName = "jobSubmit@";
	serviceName.append(mmachineId);
	diet_profile_t* submitJobProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
	std::string msgErrorDiet = "call of function diet_string_set is rejected ";

	//IN Parameters
	if (diet_string_set(diet_parameter(submitJobProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,2), const_cast<char*>(scriptContent.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+scriptContent;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	string optionsToString = _ser.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options_));

	if (diet_string_set(diet_parameter(submitJobProfile,3), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+std::string(optionsToString);
		raiseDietMsgException(msgErrorDiet);
	}

	_ser.resetSerializer();
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str();

	if (diet_string_set(diet_parameter(submitJobProfile,4), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(submitJobProfile,5), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(submitJobProfile,6), NULL, DIET_VOLATILE);

	// Send input files, if there is any one
	CpFileOptions copts;
	copts.setIsRecursive(true) ;
	copts.setTrCommand(0);
	string inputFiles = sendInputFiles(sessionKey, options.getFileParams(), mmachineId, copts) ;
	options_.setFileParams(inputFiles);

	char* cresultMsg = NULL;
	char* errorInfo = NULL;
	if(!diet_call(submitJobProfile)) {
		if(diet_string_get(diet_parameter(submitJobProfile,5), &cresultMsg, NULL)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(diet_parameter(submitJobProfile,6), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	} else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	TMS_Data::Job_ptr job_ptr = NULL;
	string serializedJob = string(cresultMsg) ;
	parseEmfObject(serializedJob, job_ptr);
	mjob = *job_ptr;
	delete job_ptr;

	diet_profile_free(submitJobProfile);
	return 0;
}

/**
 * \brief Function to cancel job
 * \return raises an exception on error
 */
int
JobProxy::cancelJob() {

	diet_profile_t* cancelJobProfile = NULL;
	std::string sessionKey;
	char* errorInfo = NULL;
	std::string serviceName = "jobCancel@";
	serviceName.append(mmachineId);

	cancelJobProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(cancelJobProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(cancelJobProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the job object in to optionsInString
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(diet_parameter(cancelJobProfile,2), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+jobToString;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(cancelJobProfile,3), NULL, DIET_VOLATILE);

	if(!diet_call(cancelJobProfile)) {
		if(diet_string_get(diet_parameter(cancelJobProfile,3), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	}
	else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	diet_profile_free(cancelJobProfile);
	return 0;
}


/**
 * \brief Function to get job information
 * \param options the options to submit job
 * \return raises an exception on error
 */
TMS_Data::Job
JobProxy::getJobInfo() {

	diet_profile_t* getJobInfoProfile = NULL;
	std::string sessionKey;
	char* jobInString = NULL;
	char* errorInfo = NULL;
	std::string serviceName = "jobInfo@";
	serviceName.append(mmachineId);

	getJobInfoProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getJobInfoProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getJobInfoProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}


	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	std::string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(diet_parameter(getJobInfoProfile,2), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(getJobInfoProfile,3), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(getJobInfoProfile,4), NULL, DIET_VOLATILE);

	if(!diet_call(getJobInfoProfile)) {
		if(diet_string_get(diet_parameter(getJobInfoProfile,3), &jobInString, NULL)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(diet_parameter(getJobInfoProfile,4), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	}
	else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	TMS_Data::Job_ptr job_ptr = NULL;

	parseEmfObject(std::string(jobInString), job_ptr);

	mjob = *job_ptr;
	delete job_ptr;

	diet_profile_free(getJobInfoProfile);
	return mjob;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job
JobProxy::getData() const{
	return mjob;
}

/**
 * \brief Destructor
 */
JobProxy::~JobProxy() {
}
