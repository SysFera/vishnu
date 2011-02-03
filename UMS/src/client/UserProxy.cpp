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
 
UserProxy::UserProxy(const SessionProxy& session)
{
}
 
UserProxy::UserProxy(const UMS_Data::User& user)
{
}
 
int UserProxy::add(const UMS_Data::User& newUser)
{
	return 0;
}
 
int UserProxy::update(UMS_Data::User& user)
{
	return 0;
}
 
int UserProxy::deleteUser(const UMS_Data::User& user)
{
	return 0;
}
 
int UserProxy::changePassword(const std::string& newPassword)
{
	return 0;
}
 
int UserProxy::resetPassword(UMS_Data::User& user)
{
	return 0;
}
 
UMS_Data::User UserProxy::getData() const
{
  return m_user;
}
 
SessionProxy UserProxy::getSessionProxy() const
{
	return  SessionProxy();
}

UserProxy::~UserProxy()
{
}
