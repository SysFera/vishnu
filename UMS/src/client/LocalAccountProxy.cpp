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

  std::string localAccountToString;

  diet_profile_t* profile;
  if (isNewLocalAccount) {
    profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTCREATE], 2);
  } else {
    profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTUPDATE], 3);
  }

  //To serialize the mlocalAccount object in to localAccountToString
  ::ecorecpp::serializer::serializer _ser;
  localAccountToString =  _ser.serialize_str(const_cast<UMS_Data::LocalAccount_ptr>(&mlocalAccount));

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, localAccountToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

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

  std::string userId;
  std::string machineId;

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[LOCALACCOUNTDELETE], 3);
  userId = mlocalAccount.getUserId();
  machineId = mlocalAccount.getMachineId();

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, userId);
  diet_string_set(profile, 2, machineId);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

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
 * \brief Destructor, raises an exception on error
 */
LocalAccountProxy::~LocalAccountProxy() {
}
