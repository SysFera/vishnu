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
 #include "UMSVishnuException.hh"
 #include "utilsClient.hpp"

  SessionProxy::SessionProxy(const std::string& sessionKey):msessionKey(sessionKey)
  {
  }
  
  SessionProxy::SessionProxy(const UMS_Data::Session& session):msession(session)
  {
  }
  
  SessionProxy::SessionProxy()
  {
  }
  
  int SessionProxy::_connect(const UserProxy& user, bool connect, const UMS_Data::ConnectOptions& options)
  {

    diet_profile_t* profile = NULL; 
    char hostname[HOST_NAME_MAX_SIZE];
    std::string sshKey;
    std::string optionsToString;
    std::string sessionId ;
    char* sessionkey;
    char* errorInfo;
    std::string msg = "call of function diet_string_set is rejected ";

    sshKey = std::string(getenv("HOME"))+"/.ssh/id_rsa";
    gethostname(hostname, HOST_NAME_MAX_SIZE);  

    if((options.getClosePolicy() < 0) || (options.getClosePolicy() > 2)) {
       throw std::runtime_error("Invalid ClosePolicy value");
    }
 
    if(connect) {
      // SERIALIZE DATA MODEL
      const char* name = "sessionConnect";
      ::ecorecpp::serializer::serializer _ser(name);
      optionsToString =  _ser.serialize(const_cast<UMS_Data::ConnectOptions_ptr>(&options));
    }

    if(connect) {
      profile = diet_profile_alloc("sessionConnect", 4, 4, 6);
    } else {
      profile = diet_profile_alloc("sessionReconnect", 4, 4, 6);
    }
    
    //IN Parameters
    if(diet_string_set(diet_parameter(profile,0), strdup((user.getData().getUserId()).c_str()), DIET_VOLATILE)) {
       msg += "with userId parameter "+user.getData().getUserId();
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
    }
    if(diet_string_set(diet_parameter(profile,1), strdup((user.getData().getPassword()).c_str()), DIET_VOLATILE)) {
       msg += "with password parameter";
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
    }
    if(diet_string_set(diet_parameter(profile,2), strdup(sshKey.c_str()), DIET_VOLATILE)) {
       msg += "with sshKey parameter "+sshKey;
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
    }  
    if(diet_string_set(diet_parameter(profile,3), hostname, DIET_VOLATILE)) {
       msg += "with hostname parameter "+std::string(hostname);
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
    }  
    if(connect) {
      if(diet_string_set(diet_parameter(profile,4), strdup(optionsToString.c_str()), DIET_VOLATILE)){
       msg += "with optionsToString parameter ";
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
      } 
    } else {
      sessionId = msession.getSessionId();
      if(diet_string_set(diet_parameter(profile,4), strdup(sessionId.c_str()), DIET_VOLATILE)) {
        msg += "with sessionId parameter "+sessionId;
        ERRMSG(msg.c_str());
        sendErrorMsg(msg);
      }
    }
    //OUT Parameters
    diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,6), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
	if(diet_string_get(diet_parameter(profile,5), &sessionkey, NULL)){
           msg += " by receiving sessionKey value";
           ERRMSG(msg.c_str());
           sendErrorMsg(msg);
        }
	if(diet_string_get(diet_parameter(profile,6), &errorInfo, NULL)) {
           msg += " to receiving errorInfo message";
           ERRMSG(msg.c_str());
           sendErrorMsg(msg);
        }
  
	msession.setSessionKey(sessionkey);
        if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
    } else {
       sendErrorMsg(" the function diet_call is rejected");
    }

     /*To check the receiving message error*/
     checkErrorMsg(errorInfo);

    return 0;
  }

  int SessionProxy::connect(const UserProxy& user, const UMS_Data::ConnectOptions& options) 
  {
    return _connect(user, true, options);
  };
  
  int SessionProxy::reconnect(const UserProxy& user)
  {
    return _connect(user, false);;
  }
  
  int SessionProxy::close()
  {

    char* errorInfo = NULL;
    std::string msg = "call of function diet_string_set is rejected ";

    std::string sessionKey =  msessionKey;

    diet_profile_t* profile = diet_profile_alloc("sessionClose", 0, 0, 1); 
    //IN Parameters
    if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       msg += "with sessionKey parameter "+sessionKey;
       ERRMSG(msg.c_str());
       sendErrorMsg(msg);
    }

    //OUT Parameters
    diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,1), &errorInfo, NULL)) {
         msg += " by receiving errorInfo message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
      }
      
      if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
  
   } else {  
      sendErrorMsg(" the function diet_call is rejected"); 
   }
   
     /*To check the receiving message error*/
     checkErrorMsg(errorInfo);

   return 0;
  }
 
  std::string SessionProxy::getSessionKey() const
  {
   return msessionKey;
  }
 
  UMS_Data::Session SessionProxy::getData() const
  {
    return msession;
  }

  SessionProxy::~SessionProxy()
  {
  }
