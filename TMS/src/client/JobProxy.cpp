
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
	if (diet_string_set(submitJobProfile,0, sessionKey.c_str())) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(submitJobProfile,1, mmachineId.c_str())) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(submitJobProfile,2, scriptContent.c_str())) {
		msgErrorDiet += "with optionsInString parameter "+scriptContent;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	string optionsToString = _ser.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options_));

	if (diet_string_set(submitJobProfile,3, optionsToString.c_str())) {
		msgErrorDiet += "with optionsInString parameter "+std::string(optionsToString);
		raiseDietMsgException(msgErrorDiet);
	}

	_ser.resetSerializer();
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str();

	if (diet_string_set(submitJobProfile,4, jobToString.c_str())) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(submitJobProfile,5);
	diet_string_set(submitJobProfile,6);

        // FIXME: do it before setting parameter 3
	// Send input files, if there is any one
	CpFileOptions copts;
	copts.setIsRecursive(true) ;
	copts.setTrCommand(0);
	std::cout << "I am HERE " << std::endl;
	string inputFiles = sendInputFiles(sessionKey, options.getFileParams(), mmachineId, copts) ;
	options_.setFileParams(inputFiles);
    std::cout << "I am HERE2 " << inputFiles << std::endl;
        std::string cresultMsg;
        std::string errorInfo;
	if(!diet_call(submitJobProfile)) {
		if(diet_string_get(submitJobProfile,5, cresultMsg)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(submitJobProfile,6, errorInfo)){
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
        std::string errorInfo;
	std::string serviceName = "jobCancel@";
	serviceName.append(mmachineId);

	cancelJobProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(cancelJobProfile,0, sessionKey.c_str())) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(cancelJobProfile,1, mmachineId.c_str())) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the job object in to optionsInString
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(cancelJobProfile,2, jobToString.c_str())) {
		msgErrorDiet += "with jobInString parameter "+jobToString;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(cancelJobProfile,3);

	if(!diet_call(cancelJobProfile)) {
		if(diet_string_get(cancelJobProfile,3, errorInfo)){
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
        std::string jobInString;
        std::string errorInfo;
	std::string serviceName = "jobInfo@";
	serviceName.append(mmachineId);

	getJobInfoProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(getJobInfoProfile,0, sessionKey.c_str())) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(getJobInfoProfile,1, mmachineId.c_str())) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}


	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	std::string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(getJobInfoProfile,2, jobToString.c_str())) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(getJobInfoProfile,3);
	diet_string_set(getJobInfoProfile,4);

	if(!diet_call(getJobInfoProfile)) {
		if(diet_string_get(getJobInfoProfile,3, jobInString)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(getJobInfoProfile,4, errorInfo)){
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
