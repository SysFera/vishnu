#include <iostream>
#include <string>
#include "ProcessCtlProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "TMS_Data.hpp"
#include "api_fms.hpp"

using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \param machineId The id of the machine
* \brief Constructor
*/
ProcessCtlProxy::ProcessCtlProxy( const SessionProxy& session):msessionProxy(session) {
}


/**
* \param session The object which encapsulates the session information
* \param machineId The id of the machine
* \brief Constructor
*/
ProcessCtlProxy::ProcessCtlProxy( const SessionProxy& session,
			  const std::string& machineId)
  :msessionProxy(session), mmachineId(machineId) {
}


/**
* \brief Function to to restart a SeD or a MA
* \param options the options for the restart
* \return raises an exception on error
*/
int
ProcessCtlProxy::restart(const IMS_Data::RestartOp& options) {

  diet_profile_t* restartProfile = NULL;
  std::string sessionKey;
  std::string restartOpToString;
  char* errorInfo = NULL;

  std::string serviceName = "int_restart";

  restartProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(restartProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(restartProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  restartOpToString =  strdup(_ser.serialize_str(const_cast<IMS_Data::RestartOp_ptr>(&options)).c_str());

  if (diet_string_set(diet_parameter(restartProfile,2), strdup(restartOpToString.c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(restartProfile,3), NULL, DIET_VOLATILE);

  if(!diet_call(restartProfile)) {
    if(diet_string_get(diet_parameter(restartProfile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(restartProfile);
  return 0;
}

/**
* \brief Function to get the refresh period
* \param process The data structure containing information about the process to stop
* \return raises an exception on error
*/
int
ProcessCtlProxy::stop(IMS_Data::Process process) {

   diet_profile_t* stopProfile = NULL;
  std::string sessionKey;
  char* errorInfo = NULL;
  std::string processToString;

  std::string serviceName = "int_stop";

  stopProfile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(stopProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  processToString =  strdup(_ser.serialize_str(const_cast<IMS_Data::Process_ptr>(&process)).c_str());

  if (diet_string_set(diet_parameter(stopProfile,1), strdup(processToString.c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(stopProfile,2), NULL, DIET_VOLATILE);

  if(!diet_call(stopProfile)) {
    if(diet_string_get(diet_parameter(stopProfile,2), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(stopProfile);

  return 0;
}

/**
* \brief Function to get the refresh period
* \param loadShedType the type of the load shedding mode (SOFT or HARD)
* \return raises an exception on error
*/
int
ProcessCtlProxy::loadShed(IMS_Data::LoadShedType loadShedType) {

  // Cancelling FMS transfer
  try {
    // If no FMS sed, catching exception and do nothing
  cancelFMS();
  } catch (UserException& e) {
    //    throw (e);
  } catch (SystemException& e) {
    //    throw (e);
  }
  // Cancelling TMS jobs
  try {
    // If no TMS sed, catching exception and do nothing
    cancelTMS();
  } catch (VishnuException& e) {
  } 

  // If hard load shedding
  if (loadShedType == 1) {
    diet_profile_t* loadShedProfile = NULL;
    std::string sessionKey;
    char* errorInfo = NULL;

     std::string serviceName = "int_loadShed";
    loadShedProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
    sessionKey = msessionProxy.getSessionKey();

    std::string msgErrorDiet = "call of function diet_string_set is rejected ";
    //IN Parameters
    if (diet_string_set(diet_parameter(loadShedProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msgErrorDiet += "with sessionKey parameter "+sessionKey;
      raiseDietMsgException(msgErrorDiet);
    }

    if (diet_string_set(diet_parameter(loadShedProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
      msgErrorDiet += "with machineId parameter "+mmachineId;
      raiseDietMsgException(msgErrorDiet);
    }

    if (diet_string_set(diet_parameter(loadShedProfile,2), strdup(convertToString(loadShedType).c_str()), DIET_VOLATILE)) {
      msgErrorDiet += "with SystemInfo parameter ";
      raiseDietMsgException(msgErrorDiet);
    }

    //OUT Parameters
    diet_string_set(diet_parameter(loadShedProfile,3), NULL, DIET_VOLATILE);

    if(!diet_call(loadShedProfile)) {
      if(diet_string_get(diet_parameter(loadShedProfile,3), &errorInfo, NULL)){
	msgErrorDiet += " by receiving errorInfo message";
	raiseDietMsgException(msgErrorDiet);
      }
    }
    else {
      raiseDietMsgException("DIET call failure");
    }

    /*To raise a vishnu exception if the receiving message is not empty*/
    raiseExceptionIfNotEmptyMsg(errorInfo);

    diet_profile_free(loadShedProfile);
  }
  return 0;
}

/**
* \brief Destructor
*/
ProcessCtlProxy::~ProcessCtlProxy() {
}

void
ProcessCtlProxy::cancelFMS() {
#ifdef COMPILE_FMS
  FMS_Data::StopTransferOptions op = FMS_Data::StopTransferOptions();
  op.setFromMachineId(mmachineId);
  op.setTransferId("all");
  op.setUserId("all");

  stopFileTransfer(msessionProxy.getSessionKey(), op);
#endif
}

void
ProcessCtlProxy::cancelTMS() {
#ifdef COMPILE_TMS
  diet_profile_t* cancelTMSProfile = NULL;
  std::string sessionKey;
  char* jobToString = NULL;
  char* errorInfo = NULL;
  std::string serviceName = "jobCancel_";
  TMS_Data::Job job;
  job.setJobId("all");

  serviceName.append(mmachineId);

  cancelTMSProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(cancelTMSProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(cancelTMSProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the job object in to optionsInString
  jobToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&job)).c_str());

  if (diet_string_set(diet_parameter(cancelTMSProfile,2), jobToString, DIET_VOLATILE)) {
    msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(cancelTMSProfile,3), NULL, DIET_VOLATILE);

  if(!diet_call(cancelTMSProfile)) {
    if(diet_string_get(diet_parameter(cancelTMSProfile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(cancelTMSProfile);
#endif
}

