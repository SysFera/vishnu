/**
 * \file AuthAccountProxy.cpp
 * \brief This file contains the VISHNU AuthAccountProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"
#include "AuthAccountProxy.hpp"

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

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string authAccountToString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(name.c_str(), 1, 1, 2);

  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the mauthAccount object in to authAccountToString
  authAccountToString =  _ser.serialize_str(const_cast<UMS_Data::AuthAccount_ptr>(&mauthAccount));

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), const_cast<char*>(authAccountToString.c_str()), DIET_VOLATILE)) {
    msg += "with authAccountToString parameter "+authAccountToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
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
 * \brief Function to add an user authentication account
 * \return raises an exception on error
 */
int
AuthAccountProxy::add()
{
  return _addAuthAccountInformation("authAccountCreate");
}

/**
 * \brief Function to update an user authentication account
 * \return raises an exception on error
 */
int
AuthAccountProxy::update()
{
  return _addAuthAccountInformation("authAccountUpdate");
}

/**
 * \brief Function to removes an user authentication account (for a given user on a given machine) from VISHNU
 * \return raises an exception on error
 */
int
AuthAccountProxy::deleteAuthAccount()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userId;
  std::string sysId;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("authAccountDelete", 2, 2, 3);
  sessionKey = msessionProxy.getSessionKey();
  userId = mauthAccount.getUserId();
  sysId = mauthAccount.getAuthSystemId();

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), const_cast<char*>(sysId.c_str()), DIET_VOLATILE)) {
    msg += "with systemId parameter "+sysId;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,2), const_cast<char*>(userId.c_str()), DIET_VOLATILE)) {
    msg += "with userId parameter "+userId;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
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
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \fn SessionProxy getSessionProxy()
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
