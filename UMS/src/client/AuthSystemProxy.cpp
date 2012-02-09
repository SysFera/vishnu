/**
 * \file AuthSystemProxy.cpp
 * \brief This file contains the VISHNU AuthSystemProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"
#include "AuthSystemProxy.hpp"

/**
 * \param authSystem The object which encapsulates the user aythentication system
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor
 */
AuthSystemProxy::AuthSystemProxy(const UMS_Data::AuthSystem& authSystem, const SessionProxy& session):
  mauthSystem(authSystem), msessionProxy(session)
{
}


/**
 * \brief Function to add an authentication system
 * \return raises an exception on error
 */
int
AuthSystemProxy::add() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string authSystemToString;
  char* authSystemInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("systemAuthCreate", 1, 1, 3);

  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the mauthSystem object in to authSystemToString
  authSystemToString =  _ser.serialize_str(const_cast<UMS_Data::AuthSystem_ptr>(&mauthSystem));

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(authSystemToString.c_str()), DIET_VOLATILE)) {
    msg += "with authSystemToString parameter "+authSystemToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {

    if(diet_string_get(diet_parameter(profile,2), &authSystemInString, NULL)) {
      msg += "with authSystemInString parameter "+std::string(authSystemInString);
      raiseDietMsgException(msg);
    }

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

  UMS_Data::AuthSystem_ptr authSystem_ptr;

  //To parse User object serialized
  parseEmfObject(std::string(authSystemInString), authSystem_ptr,
                 "Error by receiving AuthSystem object serialized");

  mauthSystem = *authSystem_ptr;
  delete authSystem_ptr;

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to update an authentication system
 * \return raises an exception on error
 */
int
AuthSystemProxy::update() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string authSystemToString;
  char* errorInfo;
  char* authSystemInString;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("systemAuthUpdate", 1, 1, 2);

  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the mauthSystem object in to authSystemToString
  authSystemToString =  _ser.serialize_str(const_cast<UMS_Data::AuthSystem_ptr>(&mauthSystem));

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(authSystemToString.c_str()), DIET_VOLATILE)) {
    msg += "with authSystemToString parameter "+authSystemToString;
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
 * \brief Function to removes an authentication system
 * \return raises an exception on error
 */
int
AuthSystemProxy::deleteAuthSystem()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string sysId;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("systemAuthDelete", 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  sysId = mauthSystem.getAuthSystemId();

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(sysId.c_str()), DIET_VOLATILE)) {
    msg += "with systemId parameter "+sysId;
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
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \fn SessionProxy getSessionProxy()
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy AuthSystemProxy::getSessionProxy() const
{
  return msessionProxy;
}

/**
 * \brief Destructor, raises an exception on error
 */
AuthSystemProxy::~AuthSystemProxy()
{
}
