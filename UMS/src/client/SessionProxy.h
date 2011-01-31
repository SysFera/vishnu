#ifndef SESSION_PROXY_H
#define SESSION_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//class SessionProxy;
class UserProxy;

#include "UserProxy.h"
#include "Session.h"
#include "ConnectOptions.hpp"


class SessionProxy
{

private:
	UMS_Data::Session session;
 
public:
        //constructor
        SessionProxy();
	SessionProxy(std::string sessionKey);
	SessionProxy(UMS_Data::Session session);

        //methods
	int connect(UserProxy user, UMS_Data::ConnectOptions options);
	int reconnect(UserProxy user);
	int close();
	UMS_Data::Session getData();

        //Destructor
	~SessionProxy();
};
#endif
