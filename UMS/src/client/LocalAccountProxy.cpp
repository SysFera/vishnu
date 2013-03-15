/**
 * \file LocalAccountProxy.cpp
 * \brief This file contains the VISHNU LocalAccountProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */

#include "LocalAccountProxy.hpp"

#include "DIET_client.h"                // for diet_string_set, etc

#include "ecorecpp/serializer/serializer.hpp"  // for serializer
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc
#include "UMSServices.hpp"

/**
 * \brief Constructor, raises an exception on error
 * \param localAccount The object which encapsulates the user Acccount information
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 */
LocalAccountProxy::LocalAccountProxy(const UMS_Data::LocalAccount& localAccount,
                                     const SessionProxy& session)
 : mlocalAccount(localAccount), msessionProxy(session) {}

/**
 * \brief Function to combine add() and update() into one function
 * \param isNewLocalAccount to select the call of add or update function
 * \return raises an exception on error
 */
int
LocalAccountProxy::_addLocalAccountInformation(bool isNewLocalAccount) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string localAccountToString;
  std::string sshPublicKey;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  if(isNewLocalAccount) {
    profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTCREATE], 1, 1, 3);
  }
  else {
    profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTUPDATE], 1, 1, 2);
  }

  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the mlocalAccount object in to localAccountToString
  localAccountToString =  _ser.serialize_str(const_cast<UMS_Data::LocalAccount_ptr>(&mlocalAccount));

  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(profile, 1, localAccountToString)) {
    msg += "with localAccountToString parameter "+localAccountToString;
    raiseCommunicationMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,2);
  if(isNewLocalAccount) {
    diet_string_set(profile,3);
  }

  if(!diet_call(profile)) {
    if(isNewLocalAccount) {
      if(diet_string_get(profile,2, sshPublicKey)){
        msg += "by receiving sshPluciKey content";
        raiseCommunicationMsgException(msg);
      }
      if(diet_string_get(profile,3, errorInfo)){
        msg += "by receiving errorInfo message";
        raiseCommunicationMsgException(msg);
      }
      msshPublicKey = sshPublicKey;
    }
    else {
      if(diet_string_get(profile,2, errorInfo)){
        msg += "by receiving errorInfo message";
        raiseCommunicationMsgException(msg);
      }
    }
  }
  else {
    raiseCommunicationMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to add a new local user configuration
 * \return raises an exception on error
 */
int
LocalAccountProxy::add() {
  return _addLocalAccountInformation();
}

/**
 * \brief Function to update a new local user configuration
 * \return raises an exception on error
 */
int LocalAccountProxy::update()
{
  return _addLocalAccountInformation(false);
}

/**
 * \brief Function to removes a local user configuration (for a given user on a given machine) from VISHNU
 * \return raises an exception on error
 */
int
LocalAccountProxy::deleteLocalAccount() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userId;
  std::string machineId;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTDELETE], 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();
  userId = mlocalAccount.getUserId();
  machineId = mlocalAccount.getMachineId();

  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(profile, 1, userId)) {
    msg += "with userId parameter "+userId;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(profile, 2, machineId)) {
    msg += "with machineId parameter "+machineId;
    raiseCommunicationMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,3);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,3, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseCommunicationMsgException(msg);
    }
  }
  else {
    raiseCommunicationMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy
LocalAccountProxy::getSessionProxy() const {
  return msessionProxy;
}

/**
 * \brief Function get the VISHNU ssh public key
 * \return the VISHNU ssh public Key
 * \return raises an exception on error
 */
std::string
LocalAccountProxy::getSshPublicKey() const {
  return msshPublicKey;
}

/**
 * \brief Destructor, raises an exception on error
 */
LocalAccountProxy::~LocalAccountProxy() {
}
