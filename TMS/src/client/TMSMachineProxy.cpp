
#include <iostream>
#include <string>
#include "TMSMachineProxy.hpp"
#include "utilsClient.hpp"
#include "utilsTMSClient.hpp"
#include "emfTMSUtils.hpp"

using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \param machineId The id of the machine
* \brief Constructor
*/
TMSMachineProxy::TMSMachineProxy( const SessionProxy& session,
                          const std::string& machineId)
  :msessionProxy(session), mmachineId(machineId) {
}

/**
* \brief Function to get the list of queues
* \return A ListQueues data structure
*/
TMS_Data::ListQueues_ptr
TMSMachineProxy::getMachineQueues() {

  diet_profile_t* profile = NULL;
  std::string sessionKey;

  char* listQueuesInString;
  char* errorInfo;
  TMS_Data::ListQueues_ptr listQueues;

  std::string serviceName = "TMSMachineGetListOfQueues_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 3);
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

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &listQueuesInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  TMSUtils::raiseTMSExceptionIfNotEmptyMsg(errorInfo);

  //To parse ListQueues object serialized
  if (!vishnu::parseTMSEmfObject(std::string(listQueuesInString), listQueues, "Error when receiving ListQueues object serialized")) {
    throw UserException(ERRCODE_INVALID_PARAM);
  }

  return listQueues;
}


/**
* \brief Function to set a refresh period
* \param refreshPeriod the refresh period
* \return raises an exception on error
*/
void
TMSMachineProxy::setMachineRefreshPeriod(int refreshPeriod) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo;

  std::string serviceName = "TMSMachineRefreshPeriodSet_";
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

  if (diet_string_set(diet_parameter(profile,2), strdup(vishnu::convertToString(refreshPeriod).c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with refreshPeriod parameter "+vishnu::convertToString(refreshPeriod);
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
  TMSUtils::raiseTMSExceptionIfNotEmptyMsg(errorInfo);

  mrefreshPeriod = refreshPeriod;

}

/**
* \brief Function to get the refresh period
* \param value The new refresh period value
* \return raises an exception on error
*/

int
TMSMachineProxy::getMachineRefreshPeriod(int& value) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo;
  std::string refreshPeriod;

  std::string serviceName = "TMSMachineRefreshPeriodGet_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 3);
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

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {

    if(diet_string_get(diet_parameter(profile,2), &refreshPeriod, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }

    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  mrefreshPeriod = vishnu::convertToInt(refreshPeriod);
  value = mrefreshPeriod;

  return 0;
}


/**
* \brief Destructor
*/
TMSMachineProxy::~TMSMachineProxy() {
}
