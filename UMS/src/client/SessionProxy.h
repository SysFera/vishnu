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
#include "Session.hpp"
#include "ConnectOptions.hpp"

#define HOST_NAME_MAX_SIZE 256

class SessionProxy
{

public:
        //constructor
        SessionProxy();
	explicit SessionProxy(const std::string& sessionKey);
	explicit SessionProxy(const UMS_Data::Session& session);

        //methods
	int connect(const UserProxy& user, const UMS_Data::ConnectOptions& options);
	int reconnect(const UserProxy& user);
	int close();
        std::string getSessionKey() const;
	UMS_Data::Session getData() const;
        std::string getErrorInfo() const;

        //Destructor
	~SessionProxy();
private:
       
       std::string serialize(const UMS_Data::ConnectOptions& options, 
                              UMS_Data::ConnectOptions_ptr& serializedOptions);

       int _connect(const UserProxy& user, bool connect, const UMS_Data::ConnectOptions& options=UMS_Data::ConnectOptions());

       //Attributs      
       UMS_Data::Session msession;
       std::string msessionKey;
       std::string merrorInfo;
};
#endif
