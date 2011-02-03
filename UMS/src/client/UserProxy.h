#ifndef USER_PROXY_H
#define USER_PROXY_H

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
private:
	UMS_Data::User m_user;
	SessionProxy* sessionProxy;
//	SessionProxy sessionProxy;

public:
        //Constructors	
        UserProxy(const std::string& userId, const std::string& password);
	explicit UserProxy(const SessionProxy& session);
	explicit UserProxy(const UMS_Data::User& user);

        //methods	
        int add(const UMS_Data::User& newUser);
	int update(UMS_Data::User& user);
	int deleteUser(const UMS_Data::User& user);
	int changePassword(const std::string& newPassword);
	int resetPassword(UMS_Data::User& user);
	UMS_Data::User getData() const;
        SessionProxy getSessionProxy() const;

        //Destructor	
        ~UserProxy();
};
#endif
