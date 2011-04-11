
#include <iostream>
#include <string>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"


/**
* \param session The object which encapsulates the session information
* \param machine The object which encapsulates the machine information
* \param options the options to submit job
* \brief Constructor, raises an exception on error
*/
JobProxy::JobProxy(const SessionProxy& session,
                   const MachineProxy& machine)
  :msessionProxy(session), mmachineProxy(machine) {

}

/**
* \param session The object which encapsulates the session information
* \param machine The object which encapsulates the machine information
* \param job The job data structure
* \param options the options to submit job
* \brief Constructor, raises an exception on error
*/
JobProxy::JobProxy(const SessionProxy& session,
                   const MachineProxy& machine,
                   TMS_Data::Job& job)
  :msessionProxy(session), mmachineProxy(machine), mjob(job) {

}

/**
* \brief Function to submit job
* \param options the options to submit job
* \return raises an exception on error
*/
int
JobProxy::submitJob(const TMS_Data::SubmitOptions& options) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* optionsToString;
  char* jobToString;
  char* jobInString;
  char* errorInfo;
  std::string serviceName = "jobSubmit_";
  serviceName.append(mmachineProxy.getData().getMachineId());

  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
  sessionKey = msessionProxy.getSessionKey();

  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineProxy.getData().getMachineId().c_str()), DIET_VOLATILE)) {
    msg += "with machineId parameter "+mmachineProxy.getData().getMachineId();
    raiseDietMsgException(msg);
  }

  const char* name = "submit";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  optionsToString =  strdup(_ser.serialize(const_cast<TMS_Data::SubmitOptions_ptr>(&options)).c_str());

  if (diet_string_set(diet_parameter(profile,2), optionsToString, DIET_VOLATILE)) {
    msg += "with optionsInString parameter "+std::string(optionsToString);
    raiseDietMsgException(msg);
  }

  _ser.resetSerializer();

  //To serialize the job object in to optionsInString
  jobToString =  strdup(_ser.serialize(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

  if (diet_string_set(diet_parameter(profile,3), jobToString, DIET_VOLATILE)) {
    msg += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msg);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,4), &jobInString, NULL)){
      msg += " by receiving User serialized  message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(diet_parameter(profile,5), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  TMS_Data::Job_ptr job_ptr;

  //To parse User object serialized
  parseEmfObject(std::string(jobInString), job_ptr, "Error by receiving Job object serialized");
  mjob = *job_ptr;

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
  char* jobToString;
  char* errorInfo;
  std::string serviceName = "jobCancel_";
  serviceName.append(mmachineProxy.getData().getMachineId());

  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();

  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineProxy.getData().getMachineId().c_str()), DIET_VOLATILE)) {
    msg += "with machineId parameter "+mmachineProxy.getData().getMachineId();
    raiseDietMsgException(msg);
  }

  const char* name = "cancel";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the job object in to optionsInString
  jobToString =  strdup(_ser.serialize(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str());

  if (diet_string_set(diet_parameter(profile,2), jobToString, DIET_VOLATILE)) {
    msg += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

	return 0;
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
  * \brief Destructor, raises an exception on error
  */
JobProxy::~JobProxy() {

}
