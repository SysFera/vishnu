
#include <iostream>
#include <string>
#include "ProcessCtlProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"

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

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string restartOpToString;
  char* errorInfo = NULL;

  std::string serviceName = "int_restart";

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

   const char* name = "int_restart";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  restartOpToString =  strdup(_ser.serialize(const_cast<IMS_Data::RestartOp_ptr>(&options)).c_str());

  if (diet_string_set(diet_parameter(profile,2), strdup(restartOpToString.c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
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
* \brief Function to get the refresh period
* \param process The data structure containing information about the process to stop
* \return raises an exception on error
*/
int
ProcessCtlProxy::stop(IMS_Data::Process process) {

   diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo = NULL;
  std::string processToString;

  std::string serviceName = "int_stop";

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  const char* name = "setSystemInfo";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  processToString =  strdup(_ser.serialize(const_cast<IMS_Data::Process_ptr>(&process)).c_str());

  if (diet_string_set(diet_parameter(profile,1), strdup(processToString.c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
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
* \brief Function to get the refresh period
* \param loadShedType the type of the load shedding mode (SOFT or HARD)
* \return raises an exception on error
*/
int
ProcessCtlProxy::loadShed(IMS_Data::LoadShedType loadShedType) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo = NULL;

  std::string serviceName = "int_loadShed";
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

  if (diet_string_set(diet_parameter(profile,2), strdup(convertToString(loadShedType).c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
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
* \brief Destructor
*/
ProcessCtlProxy::~ProcessCtlProxy() {
}
