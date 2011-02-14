#ifndef _USER_PROXY_H_
#define _USER_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//class UserProxy;
class SessionProxy;
#include "SessionProxy.h"
#include "User.hpp"


class UserProxy
{

public:
  //Constructors	
  UserProxy(const std::string& userId, const std::string& password);
  explicit UserProxy(SessionProxy session);
  explicit UserProxy(const UMS_Data::User& user);

  //methods	
  int add(const UMS_Data::User& user);
  int update(const UMS_Data::User& user);
  int deleteUser(const UMS_Data::User& user);
  int changePassword(const std::string& password, const std::string& newPassword);
  int resetPassword(UMS_Data::User& user);
  UMS_Data::User getData() const;
  SessionProxy getSessionProxy() const;

  //Destructor	
  ~UserProxy();

private:

  int _addUserInformation(const UMS_Data::User& user, bool isNewUser=true);

  UMS_Data::User muser;
  SessionProxy* msessionProxy;

};
#endif
