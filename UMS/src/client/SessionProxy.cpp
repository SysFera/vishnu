#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "SessionProxy.h"
#include "UserProxy.h"

SessionProxy::SessionProxy(std::string sessionKey):m_sessionKey(sessionKey)
{
  m_errorInfo = "";
}
 
SessionProxy::SessionProxy(UMS_Data::Session session):m_session(session)
{
  m_errorInfo = "";
}
 
SessionProxy::SessionProxy()
{
  m_errorInfo = "";
}
 
int SessionProxy::_connect(UserProxy user, bool connect, UMS_Data::ConnectOptions options)
{

  int res = 1; 
  diet_profile_t* profile = NULL; 
  char hostname[HOST_NAME_MAX_SIZE];
  std::string sshKey;
  std::string optionsToString;
  std::string sessionId = "Todo";
  char* sessionkey;
  char* errorInfo;

  sshKey = std::string(getenv("HOME"))+"/.ssh/id_rsa";
  gethostname(hostname, HOST_NAME_MAX_SIZE);  
  //getsshKey :  a voir
 
  //Serialization of the ConnectOption
  // CREATE DATA MODEL
  if(connect) {
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    UMS_Data::ConnectOptions_ptr serializedOptions = ecoreFactory->createConnectOptions();
    serializedOptions = &options;
    // SERIALIZE DATA MODEL
    const char* name = "umsConnect";
    ::ecorecpp::serializer::serializer _ser(name);
    optionsToString =  _ser.serialize(serializedOptions);
  }

  if(connect) {
     profile = diet_profile_alloc("connect", 4, 4, 6);
  } else {
     profile = diet_profile_alloc("reconnect", 4, 4, 6);
  }
  //IN Parameters
  diet_string_set(diet_parameter(profile,0), strdup((user.getData().getUserId()).c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,1), strdup((user.getData().getPassword()).c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,2), hostname, DIET_VOLATILE);  
  diet_string_set(diet_parameter(profile,3), strdup(sshKey.c_str()), DIET_VOLATILE);  
  if(connect) {
     diet_string_set(diet_parameter(profile,4), strdup(optionsToString.c_str()), DIET_VOLATILE); 
  } else {
     //sessionId = m_session.getSessionKey();
     diet_string_set(diet_parameter(profile,4), strdup(sessionId.c_str()), DIET_VOLATILE);
  }
  //OUT Parameters
  diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,6), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
      diet_string_get(diet_parameter(profile,5), &sessionkey, NULL);
      diet_string_get(diet_parameter(profile,6), &errorInfo, NULL);
 
      m_session.setSessionKey(sessionkey);
      m_errorInfo = errorInfo;
   }

   res = (m_errorInfo.size() > 0);

  return res;
}

int SessionProxy::connect(UserProxy user, UMS_Data::ConnectOptions options) 
{
  return _connect(user, true, options);
};
 
int SessionProxy::reconnect(UserProxy user)
{
  return _connect(user, false);;
}
 
int SessionProxy::close()
{

   int res = 0;
   char* errorInfo = NULL;
   std::string sessionKey = m_sessionKey;

  diet_profile_t* profile = diet_profile_alloc("close", 0, 0, 1); 
   //IN Parameters
  diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE);

  std::cout << "+++++++++++++++sessionKey=" << strdup(sessionKey.c_str()) << "++++++++++++++" << std::endl;
  //OUT Parameters
  diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
       diet_string_get(diet_parameter(profile,1), &errorInfo, NULL);
       std::cout << "errorInfo=" << errorInfo << std::endl;
       m_errorInfo = errorInfo;
   }
  
   res = (m_errorInfo.size() > 0);

   return res;
}
 
UMS_Data::Session SessionProxy::getData()
{
  return m_session;
}

std::string SessionProxy::getErrorInfo()
{
  return m_errorInfo;
}
 
SessionProxy::~SessionProxy()
{
}

std::string SessionProxy::serialize(const UMS_Data::ConnectOptions& options, 
                                    UMS_Data::ConnectOptions_ptr& serializedOptions)
{
   /*serializedOptions->setClosePolicy(options.getClosePolicy);
   serializedOptions->setSessionInactivityDelay(options.getSessionInactivityDelay);
   serializedOptions->setSubstituteUserId(options.SubstituteUserId);

   // SERIALIZE DATA MODEL
   const char* name = "umsConnect";
   ::ecorecpp::serializer::serializer _ser(name);
 
 return _ser.serialize(serializedOptions);*/
 return "";
}
