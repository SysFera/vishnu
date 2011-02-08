#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserProxy.h"
#include "UMSVishnuException.hh"
#include "debug.hh"

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
   
   if(isNewUser) profile = diet_profile_alloc("userCreate", 1, 1, 2);
   else profile = diet_profile_alloc("userUpdate", 1, 1, 2);
   sessionKey = msessionProxy->getSessionKey();

   const char* name = "addUserInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   userToString =  _ser.serialize(const_cast<UMS_Data::User_ptr>(&user));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };
   if(diet_string_set(diet_parameter(profile,1), strdup(userToString.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }  
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

   return (strlen(errorInfo) > 0);
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

   profile = diet_profile_alloc("userDelete", 1, 1, 2);
   sessionKey = msessionProxy->getSessionKey();
   userId = user.getUserId();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };
   if(diet_string_set(diet_parameter(profile,1), strdup(userId.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

   return (strlen(errorInfo) > 0);
}
 
int UserProxy::changePassword(const std::string& newPassword)
{

   diet_profile_t* profile = NULL;
   char* errorInfo;

   profile = diet_profile_alloc("userPasswordChange", 2, 2, 3);

   //IN Parameters  
   if(diet_string_set(diet_parameter(profile,0), strdup((muser.getUserId()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   if(diet_string_set(diet_parameter(profile,1), strdup((muser.getPassword()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   if(diet_string_set(diet_parameter(profile,2), strdup(newPassword.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

  return 0;
}
 
int UserProxy::resetPassword(UMS_Data::User& user)
{

   diet_profile_t* profile = NULL;
   char* errorInfo;

   profile = diet_profile_alloc("userPasswordReset", 1, 1, 2);

   //IN Parameters  
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   if(diet_string_set(diet_parameter(profile,1), strdup((user.getUserId()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }
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
