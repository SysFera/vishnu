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
	UMS_Data::User user;
	SessionProxy* sessionProxy;
//	SessionProxy sessionProxy;
 
public:
        //Constructors	
        UserProxy(const std::string& userId, const std::string& password);
	UserProxy(SessionProxy session);
	UserProxy(UMS_Data::User user);

        //methods	
        int add(UMS_Data::User newUser);
	int update(UMS_Data::User user);
	int deleteUser(UMS_Data::User user);
	int changePassword(std::string newPassword);
	int resetPassword(UMS_Data::User user);
	UMS_Data::User getData();
        SessionProxy getSessionProxy();

        //Destructor	
        ~UserProxy();
};
#endif
