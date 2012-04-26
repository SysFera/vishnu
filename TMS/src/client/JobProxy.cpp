
#include <iostream>
#include <string>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "DIET_Dagda.h"

using namespace  std ;
// using namespace vishnu;

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

	diet_profile_t* submitJobProfile = NULL;
	std::string sessionKey;
	char* optionsToString = NULL;
	char* jobToString = NULL;
	char* jobInString = NULL;
	char* errorInfo = NULL;
	std::string serviceName = "jobSubmit_";
	serviceName.append(mmachineId);

	//submitJobProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
	submitJobProfile = diet_profile_alloc(serviceName.c_str(), 5, 5, 7);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_paramstring_set(diet_parameter(submitJobProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,2), strdup(scriptContent.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+scriptContent;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	optionsToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options)).c_str());

	if (diet_paramstring_set(diet_parameter(submitJobProfile,3), optionsToString, DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+std::string(optionsToString);
		raiseDietMsgException(msgErrorDiet);
	}

	_ser.resetSerializer();
	//To serialize the job object in to optionsInString
	jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

	if (diet_string_set(diet_parameter(submitJobProfile,4), jobToString, DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	/*  Use Dagda to send file parameters */
	if (diet_container_set(diet_parameter(submitJobProfile,5), DIET_PERSISTENT)) {
		msgErrorDiet += "with container for file parameters";
		raiseDietMsgException(msgErrorDiet);
	}

	//dagda_init_container(diet_parameter(submitJobProfile,5));
	char* idContainer = NULL ;
	dagda_create_container(&idContainer) ;
	diet_use_data(diet_parameter(submitJobProfile,5), idContainer) ;

	ListStrings paramsVec ;
	boost::split(paramsVec, options.getFileParams(), boost::is_any_of(" ")) ;
	char* DAGDA_ID[paramsVec.size()];
	int id = 0 ;
	for(ListStrings::const_iterator it = paramsVec.begin(); it != paramsVec.end(); it++){
		size_t pos = (*it).find("=") ;  //it will find normally après initial parsing
		if(pos == std::string::npos) continue ;
		std::cout << "H it : " << *it << std::endl;
		char* path = strdup( (*it).substr(pos+1, std::string::npos).c_str() ) ;
		dagda_put_file(path, DIET_PERSISTENT, &DAGDA_ID[id]);
		dagda_add_container_element((*diet_parameter(submitJobProfile,5)).desc.id, DAGDA_ID[id], id);
		id++ ;
	}
	/* End use Dagda to send file paramerers */

	//OUT Parameters
	diet_string_set(diet_parameter(submitJobProfile,6), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(submitJobProfile,7), NULL, DIET_VOLATILE);

	if(!diet_call(submitJobProfile)) {
		if(diet_string_get(diet_parameter(submitJobProfile,6), &jobInString, NULL)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(diet_parameter(submitJobProfile,7), &errorInfo, NULL)){
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
	char* jobToString = NULL;
	char* errorInfo = NULL;
	std::string serviceName = "jobCancel_";
	serviceName.append(mmachineId);

	cancelJobProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(cancelJobProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(cancelJobProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the job object in to optionsInString
	jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

	if (diet_string_set(diet_parameter(cancelJobProfile,2), jobToString, DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
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
	char* jobToString = NULL;
	char* jobInString = NULL;
	char* errorInfo = NULL;
	std::string serviceName = "jobInfo_";
	serviceName.append(mmachineId);

	getJobInfoProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getJobInfoProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getJobInfoProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}


	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

	if (diet_string_set(diet_parameter(getJobInfoProfile,2), jobToString, DIET_VOLATILE)) {
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
