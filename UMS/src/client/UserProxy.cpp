/**
 * \file UserProxy.cpp
 * \brief This file contains the VISHNU UserProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UserProxy.hpp"

#include "DIET_client.h"                // for diet_string_set, etc
#include "SessionProxy.hpp"             // for SessionProxy
#include "UserException.hpp"            // for ERRCODE_INVALID_PARAM
#include "Version.hpp"                  // for Version
#include "ecore_forward.hpp"            // for EString
#include "ecorecpp/serializer/serializer.hpp"  // for serializer
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc


#include "UMSVishnuException.hpp"
#include "vishnu_version.hpp"
#include "utilVishnu.hpp"
#include "UMSServices.hpp"

/**
 * \brief Constructor, raises an exception on error
 * \param userId the user identifier
 * \password the user password
 */
UserProxy::UserProxy(const  std::string& userId,
                     const std::string& password) {
  muser.setUserId(userId);
  muser.setPassword(password);
  msessionProxy = NULL;
}

/**
 * \brief Constructor, raises an exception on error
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 */
UserProxy::UserProxy(SessionProxy session):
  msessionProxy(&session) {
}

/**
 * \brief Constructor, raises an exception on error
 * \param user The object which encapsulates the user information
 */
UserProxy::UserProxy(const UMS_Data::User& user):
  muser(user) {
  msessionProxy = NULL;
}

/**
 * \brief Function to add new user
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::add(UMS_Data::User& user) {

  std::string sessionKey;
  std::string userInString;

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[USERCREATE], 2);
  sessionKey = msessionProxy->getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the user object in to userToString
  std::string userToString =  _ser.serialize_str(const_cast<UMS_Data::User_ptr>(&user));
  //IN Parameters
  diet_string_set(profile, 0, msessionProxy->getSessionKey());
  diet_string_set(profile, 1, userToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_string_get(profile,1, userInString);

  //To parse User object serialized
  UMS_Data::User_ptr user_ptr;
  parseEmfObject(userInString, user_ptr, "Error by receiving User object serialized");
  user = *user_ptr;
  muser = user;
  delete user_ptr;

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to update user information
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::update(const UMS_Data::User& user) {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[USERUPDATE], 2);
  //To serialize the user object in to userToString
  ::ecorecpp::serializer::serializer _ser;
  std::string userToString =  _ser.serialize_str(const_cast<UMS_Data::User_ptr>(&user));

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy->getSessionKey());
  diet_string_set(profile, 1, userToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
  return 0;
}

/**
 * \brief Function to remove user information
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::deleteUser(const UMS_Data::User& user) {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[USERDELETE], 2);

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy->getSessionKey());
  diet_string_set(profile, 1, user.getUserId());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to change user password
 * \param oldPassword the old password of the user
 * \param newPassword the new password of the user
 * \return raises an exception on error
 */
int
UserProxy::changePassword(const std::string& password,
                          const std::string& newPassword) {

  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";
  std::string versionToString;

  UMS_Data::Version_ptr versionObj = vishnu::parseVersion(VISHNU_VERSION);
  if (versionObj == NULL) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, "The format of the VISHNU VERSION is incorrect");
  } else {
    // SERIALIZE DATA MODEL
    ::ecorecpp::serializer::serializer _ser;
    //To serialize the version object in to versionToString
    versionToString =  _ser.serialize_str(versionObj);
  }

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[USERPASSWORDCHANGE], 4);
  diet_string_set(profile,0, muser.getUserId());
  diet_string_set(profile,1, password);
  diet_string_set(profile,2, newPassword);
  diet_string_set(profile,3, versionToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
  delete versionObj;

  return 0;
}

/**
 * \brief Function to reset user password
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::resetPassword(UMS_Data::User& user) {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[USERPASSWORDRESET], 2);
  diet_string_set(profile, 0, msessionProxy->getSessionKey());
  diet_string_set(profile, 1, user.getUserId());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string tmpPassword;
  diet_string_get(profile, 1, tmpPassword);

  /*To set the temporary password*/
  muser.setUserId(user.getUserId());
  muser.setPassword(tmpPassword);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function get user information
 * \return User object encapsulates the information of the user
 * \return raises an exception on error
 */
UMS_Data::User
UserProxy::getData() const {
  return muser;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy
UserProxy::getSessionProxy() const {
  return  *msessionProxy;
}

/**
 * \brief Destructor, raises an exception on error
 */
UserProxy::~UserProxy() {
}
