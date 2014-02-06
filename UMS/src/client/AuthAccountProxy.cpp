/**
 * \file AuthAccountProxy.cpp
 * \brief This file contains the VISHNU AuthAccountProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "AuthAccountProxy.hpp"

#include "DIET_client.h"                // for diet_string_set, diet_call, etc
#include "ecorecpp/serializer/serializer.hpp"  // for serializer
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc

#include "UMSServices.hpp"

/**
 * \param authAccount The object which encapsulates the user Acccount information
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor
 */
AuthAccountProxy::AuthAccountProxy(const UMS_Data::AuthAccount& authAccount, const SessionProxy& session):
  mauthAccount(authAccount), msessionProxy(session)
{
}

/**
 * \brief Function to combine add() and update() into one function
 * \param name The name of the service
 * \return raises an exception on error
 */
int
AuthAccountProxy::_addAuthAccountInformation(std::string name) {

  std::string sessionKey;
  std::string authAccountToString;
  std::string msg = "call of function diet_string_set is rejected ";

  diet_profile_t* profile = diet_profile_alloc(name, 2);

  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the mauthAccount object in to authAccountToString
  authAccountToString =  _ser.serialize_str(const_cast<UMS_Data::AuthAccount_ptr>(&mauthAccount));

  //IN Parameters
  diet_string_set(profile, 0, sessionKey);
  diet_string_set(profile, 1, authAccountToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to add an user authentication account
 * \return raises an exception on error
 */
int
AuthAccountProxy::add()
{
  return _addAuthAccountInformation(SERVICES_UMS[AUTHACCOUNTCREATE]);
}

/**
 * \brief Function to update an user authentication account
 * \return raises an exception on error
 */
int
AuthAccountProxy::update()
{
  return _addAuthAccountInformation(SERVICES_UMS[AUTHACCOUNTUPDATE]);
}

/**
 * \brief Function to removes an user authentication account (for a given user on a given machine) from VISHNU
 * \return raises an exception on error
 */
int
AuthAccountProxy::deleteAuthAccount()
{
  std::string sessionKey;
  std::string userId;
  std::string sysId;

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[AUTHACCOUNTDELETE], 3);
  sessionKey = msessionProxy.getSessionKey();
  userId = mauthAccount.getUserId();
  sysId = mauthAccount.getAuthSystemId();

  //IN Parameters
  diet_string_set(profile, 0, sessionKey);
  diet_string_set(profile, 1, sysId);
  diet_string_set(profile, 2, userId);

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
SessionProxy AuthAccountProxy::getSessionProxy() const
{
  return msessionProxy;
}

/**
 * \brief Destructor, raises an exception on error
 */
AuthAccountProxy::~AuthAccountProxy()
{
}
