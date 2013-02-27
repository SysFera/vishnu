/**
 * \file UserProxy.cpp
 * \brief This file contains the VISHNU UserProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UserProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"
#include "vishnu_version.hpp"
#include "utilVishnu.hpp"
#include "UMSServices.hpp"

/**
 * \fn  UserProxy(const std::string& userId, const std::string& password)
 * \param userId the user identifier
 * \password the user password
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(const  std::string& userId,
                     const std::string& password) {
  muser.setUserId(userId);
  muser.setPassword(password);
  msessionProxy = NULL;
}

/**
 * \fn explicit UserProxy(SessionProxy session)
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(SessionProxy session):
  msessionProxy(&session) {
}

/**
 * \fn explicit UserProxy(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(const UMS_Data::User& user):
  muser(user) {
  msessionProxy = NULL;
}

/**
 * \brief Function to add new user
 * \fn  int add(UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::add(UMS_Data::User& user) {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string errorInfo;
  std::string userInString;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(SERVICES_UMS[USERCREATE], 1, 1, 3);
  sessionKey = msessionProxy->getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the user object in to userToString
  std::string userToString =  _ser.serialize_str(const_cast<UMS_Data::User_ptr>(&user));
  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if (diet_string_set(profile, 1, userToString)) {
    msg += "with userToString parameter "+userToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,2);
  diet_string_set(profile,3);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, userInString)){
      msg += " by receiving User serialized  message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(profile,3, errorInfo)){
      msg += " by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  UMS_Data::User_ptr user_ptr;

  //To parse User object serialized
  parseEmfObject(userInString, user_ptr, "Error by receiving User object serialized");
  user = *user_ptr;
  muser = user;
  delete user_ptr;

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to update user information
 * \fn  int update(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::update(const UMS_Data::User& user) {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userToString;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(SERVICES_UMS[USERUPDATE], 1, 1, 2);

  sessionKey = msessionProxy->getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the user object in to userToString
  userToString =  _ser.serialize_str(const_cast<UMS_Data::User_ptr>(&user));

  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if (diet_string_set(profile, 1, userToString)) {
    msg += "with userToString parameter "+userToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,2);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to remove user information
 * \fn  int deleteUser(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::deleteUser(const UMS_Data::User& user) {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userId;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(SERVICES_UMS[USERDELETE], 1, 1, 2);
  sessionKey = msessionProxy->getSessionKey();
  userId = user.getUserId();

  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if (diet_string_set(profile, 1, userId)) {
    msg += "with userId parameter "+userId;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,2);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to change user password
 * \fn  int changePassword(const std::string& password, const std::string& newPassword)
 * \param oldPassword the old password of the user
 * \param newPassword the new password of the user
 * \return raises an exception on error
 */
int
UserProxy::changePassword(const std::string& password,
                          const std::string& newPassword) {
  diet_profile_t* profile = NULL;
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
  profile = diet_profile_alloc(SERVICES_UMS[USERPASSWORDCHANGE], 3, 3, 4);

  //IN Parameters
  if(diet_string_set(profile,0, muser.getUserId().c_str())) {
    msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
    raiseDietMsgException(msg);
  }

  if(diet_string_set(profile,1, password.c_str())) {
    msg += "with password parameter "+password;
    raiseDietMsgException(msg);
  }

  if(diet_string_set(profile,2, newPassword.c_str())) {
    msg += "with newPassword parameter "+newPassword;
    raiseDietMsgException(msg);
  }

  if(diet_string_set(profile,3, versionToString.c_str())) {
      msg += "with version parameter "+versionToString;
      raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile,4);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,4, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);
  delete versionObj;

  return 0;
}

/**
 * \brief Function to reset user password
 * \fn  int resetPassword(UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int
UserProxy::resetPassword(UMS_Data::User& user) {
  diet_profile_t* profile = NULL;
  std::string tmpPassword;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(SERVICES_UMS[USERPASSWORDRESET], 1, 1, 3);

  //IN Parameters
  if (diet_string_set(profile, 0, msessionProxy->getSessionKey())) {
    msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
    raiseDietMsgException(msg);
  }

  if (diet_string_set(profile, 1, user.getUserId())) {
    msg += "with userId parameter "+user.getUserId();
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(profile, 2);
  diet_string_set(profile, 3);

  if(!diet_call(profile)) {
    if(diet_string_get(profile, 2, tmpPassword)){
      msg += "by receiving tmpPassWord message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(profile, 3, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  /*To set the temporary password*/
  muser.setUserId(user.getUserId());
  muser.setPassword(tmpPassword);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function get user information
 * \fn  UMS_Data::User getData()
 * \return User object encapsulates the information of the user
 * \return raises an exception on error
 */
UMS_Data::User
UserProxy::getData() const {
  return muser;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \fn SessionProxy getSessionProxy()
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy
UserProxy::getSessionProxy() const {
  return  *msessionProxy;
}

/**
 * \fn ~UserProxy()
 * \brief Destructor, raises an exception on error
 */
UserProxy::~UserProxy() {
}
