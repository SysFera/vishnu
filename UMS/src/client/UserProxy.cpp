#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserProxy.h"


UserProxy::UserProxy(const  std::string& userId, const std::string& password)
{
}
 
UserProxy::UserProxy(SessionProxy session)
{
}
 
UserProxy::UserProxy(UMS_Data::User user)
{
}
 
int UserProxy::add(UMS_Data::User newUser)
{
	return 0;
}
 
int UserProxy::update(UMS_Data::User user)
{
	return 0;
}
 
int UserProxy::deleteUser(UMS_Data::User user)
{
	return 0;
}
 
int UserProxy::changePassword(std::string newPassword)
{
	return 0;
}
 
int UserProxy::resetPassword(UMS_Data::User user)
{
	return 0;
}
 
UMS_Data::User UserProxy::getData()
{
  return UMS_Data::User();
}
 
UserProxy::~UserProxy()
{
}
 
SessionProxy UserProxy::getSessionProxy()
{
	return  SessionProxy();
}
