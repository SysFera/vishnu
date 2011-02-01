#ifndef SESSION_PROXY_H
#define SESSION_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"

#include "DIET_client.h"

//class SessionProxy;
class UserProxy;

#include "UserProxy.h"
#include "Session.h"
#include "ConnectOptions.hpp"

#define HOST_NAME_MAX_SIZE 256

class SessionProxy
{

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
        std::string getErrorInfo();

        //Destructor
	~SessionProxy();
private:
       
       std::string serialize(const UMS_Data::ConnectOptions& options, 
                              UMS_Data::ConnectOptions_ptr& serializedOptions);

       int _connect(UserProxy user, bool connect, UMS_Data::ConnectOptions options=UMS_Data::ConnectOptions());

       //Attributs      
       UMS_Data::Session m_session;
       std::string m_sessionKey;
       std::string m_errorInfo;
};
#endif
