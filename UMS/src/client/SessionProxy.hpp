#ifndef _SESSION_PROXY_H_
#define _SESSION_PROXY_H_

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

#include "UserProxy.hpp"
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

   //Destructor
  ~SessionProxy();
private:
       
  int _connect(const UserProxy& user, bool connect, const UMS_Data::ConnectOptions& options=UMS_Data::ConnectOptions());

  //Attributs      
  UMS_Data::Session msession;
  std::string msessionKey;
};
#endif
