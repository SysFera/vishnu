#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserProxy.h"


UserProxy::UserProxy(const  std::string& userId, const std::string& password)
{
  m_user.setUserId(userId);
  m_user.setPassword(password);
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
  return m_user;
}
 
UserProxy::~UserProxy()
{
}
 
SessionProxy UserProxy::getSessionProxy()
{
	return  SessionProxy();
}
