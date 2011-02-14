#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserProxy.h"
#include "UMSVishnuException.hh"
#include "utilsClient.hpp"

UserProxy::UserProxy(const  std::string& userId, const std::string& password)
{
  muser.setUserId(userId);
  muser.setPassword(password);
}
 
UserProxy::UserProxy(SessionProxy session):
 msessionProxy(&session)
{
}
 
UserProxy::UserProxy(const UMS_Data::User& user):
 muser(user)
{
}
 
int UserProxy::_addUserInformation(const UMS_Data::User& user, bool isNewUser)
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string userToString;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";  
 
   if(isNewUser) profile = diet_profile_alloc("userCreate", 1, 1, 2);
   else profile = diet_profile_alloc("userUpdate", 1, 1, 2);
   sessionKey = msessionProxy->getSessionKey();

   const char* name = "addUserInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   userToString =  _ser.serialize(const_cast<UMS_Data::User_ptr>(&user));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(userToString.c_str()), DIET_VOLATILE)) {
     msg += "with userToString parameter "+userToString;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }
   
   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          ERRMSG(msg.c_str());
          sendErrorMsg(msg); 
       }
       if(strlen(errorInfo)==0)  std::cout << "The service was performed successfull" << std::endl;
   }  
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }
  
   /*To check the receiving message error*/ 
    checkErrorMsg(errorInfo);

   return 0;
}

int UserProxy::add(const UMS_Data::User& user) {
  return _addUserInformation(user); 
}
 
int UserProxy::update(const UMS_Data::User& user)
{
   return _addUserInformation(user, false);
}
 
int UserProxy::deleteUser(const UMS_Data::User& user)
{

   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string userId;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("userDelete", 1, 1, 2);
   sessionKey = msessionProxy->getSessionKey();
   userId = user.getUserId();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);   
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(userId.c_str()), DIET_VOLATILE)) {
     msg += "with userId parameter "+userId;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
         msg += " by receiving errorInfo message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
      }
      if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }
     /*To check the receiving message error*/
     checkErrorMsg(errorInfo);
   
  return 0;
}
 
int UserProxy::changePassword(const std::string& password, const std::string& newPassword)
{

   diet_profile_t* profile = NULL;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("userPasswordChange", 2, 2, 3);

   //IN Parameters  
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
      ERRMSG(msg.c_str());
      sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,1), strdup(password.c_str()), DIET_VOLATILE)) {
     msg += "with password parameter "+password;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,2), strdup(newPassword.c_str()), DIET_VOLATILE)) {
     msg += "with newPassword parameter "+newPassword;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          ERRMSG(msg.c_str());
          sendErrorMsg(msg);   
       }
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected"); 
   }
     /*To check the receiving message error*/   
     checkErrorMsg(errorInfo);

  return 0;
}
 
int UserProxy::resetPassword(UMS_Data::User& user)
{

   diet_profile_t* profile = NULL;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("userPasswordReset", 1, 1, 2);

   //IN Parameters  
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
      ERRMSG(msg.c_str());
      sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,1), strdup((user.getUserId()).c_str()), DIET_VOLATILE)) {
      msg += "with userId parameter "+user.getUserId();
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          ERRMSG(msg.c_str());
          sendErrorMsg(msg);     
       }
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected"); 
   }
    /*To check the receiving message error*/
    checkErrorMsg(errorInfo);

  return 0;
}
 
UMS_Data::User UserProxy::getData() const
{
  return muser;
}
 
SessionProxy UserProxy::getSessionProxy() const
{
  return  *msessionProxy;
}

UserProxy::~UserProxy()
{
}
