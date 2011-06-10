
#include <iostream>
#include <string>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilClient.hpp"

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

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* optionsToString = NULL;
  char* jobToString = NULL;
  char* jobInString = NULL;
  char* errorInfo = NULL;
  std::string serviceName = "jobSubmit_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,2), strdup(scriptContent.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with optionsInString parameter "+scriptContent;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&options)).c_str());

  if (diet_string_set(diet_parameter(profile,3), optionsToString, DIET_VOLATILE)) {
    msgErrorDiet += "with optionsInString parameter "+std::string(optionsToString);
    raiseDietMsgException(msgErrorDiet);
  }

  _ser.resetSerializer();

  //To serialize the job object in to optionsInString
  jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

  if (diet_string_set(diet_parameter(profile,4), jobToString, DIET_VOLATILE)) {
    msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,6), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,5), &jobInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
    if(diet_string_get(diet_parameter(profile,6), &errorInfo, NULL)){
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
  diet_profile_free(profile);
  return 0;
}

/**
* \brief Function to cancel job
* \return raises an exception on error
*/
int
JobProxy::cancelJob() {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* jobToString = NULL;
  char* errorInfo = NULL;
  std::string serviceName = "jobCancel_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the job object in to optionsInString
  jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

  if (diet_string_set(diet_parameter(profile,2), jobToString, DIET_VOLATILE)) {
    msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);
	return 0;
}


/**
 * \brief Function to get job information
 * \param options the options to submit job
 * \return raises an exception on error
 */
TMS_Data::Job
JobProxy::getJobInfo() {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* jobToString = NULL;
  char* jobInString = NULL;
  char* errorInfo = NULL;
  std::string serviceName = "jobInfo_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }


  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

  if (diet_string_set(diet_parameter(profile,2), jobToString, DIET_VOLATILE)) {
    msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &jobInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
    if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
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

  diet_profile_free(profile);
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
