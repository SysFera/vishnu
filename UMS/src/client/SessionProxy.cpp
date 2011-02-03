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
 #include "debug.hh"

  SessionProxy::SessionProxy(const std::string& sessionKey):msessionKey(sessionKey)
  {
    merrorInfo = "";
  }
  
  SessionProxy::SessionProxy(const UMS_Data::Session& session):msession(session)
  {
    merrorInfo = "";
  }
  
  SessionProxy::SessionProxy()
  {
    merrorInfo = "";
  }
  
  int SessionProxy::_connect(const UserProxy& user, bool connect, const UMS_Data::ConnectOptions& options)
  {

    int res = 1; 
    diet_profile_t* profile = NULL; 
    char hostname[HOST_NAME_MAX_SIZE];
    std::string sshKey;
    std::string optionsToString;
    std::string sessionId ;
    char* sessionkey;
    char* errorInfo;

    sshKey = std::string(getenv("HOME"))+"/.ssh/id_rsa";
    gethostname(hostname, HOST_NAME_MAX_SIZE);  
  
    //Serialization of the ConnectOption
    // CREATE DATA MODEL
    if(connect) {
      // SERIALIZE DATA MODEL
      const char* name = "umsConnect";
      ::ecorecpp::serializer::serializer _ser(name);
      optionsToString =  _ser.serialize(const_cast<UMS_Data::ConnectOptions_ptr>(&options));
    }

    if(connect) {
      profile = diet_profile_alloc("sessionConnect", 4, 4, 6);
    } else {
      profile = diet_profile_alloc("sessionReconnect", 4, 4, 6);
    }
    char* test = NULL;
    //IN Parameters
    if(diet_string_set(diet_parameter(profile,0), strdup((user.getData().getUserId()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
    };
    if(diet_string_set(diet_parameter(profile,1), strdup((user.getData().getPassword()).c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
    };
    if(diet_string_set(diet_parameter(profile,2), hostname, DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
    };  
    if(diet_string_set(diet_parameter(profile,3), strdup(sshKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
    }; 
    if(connect) {
      if(diet_string_set(diet_parameter(profile,4), strdup(optionsToString.c_str()), DIET_VOLATILE)){
       ERRMSG("Error in diet_string_set"); 
      } 
    } else {
      sessionId = msession.getSessionId();
      if(diet_string_set(diet_parameter(profile,4), strdup(sessionId.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
      }
    }
    //OUT Parameters
    if(diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
    }
    if(diet_string_set(diet_parameter(profile,6), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
    }

    if(!diet_call(profile)) {
	if(diet_string_get(diet_parameter(profile,5), &sessionkey, NULL)){
          ERRMSG("Error in diet_string_get");
        }
	if(diet_string_get(diet_parameter(profile,6), &errorInfo, NULL)) {
          ERRMSG("Error in diet_string_get");
        }
  
	msession.setSessionKey(sessionkey);
	merrorInfo = errorInfo;
    } else {
       ERRMSG("Error in diet_string_get");
    }

    if(merrorInfo.size() > 0 ) { 
      UMSVishnuException e(1, merrorInfo);
      throw e;
    }

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

    int res = 0;
    char* errorInfo = NULL;

    try {
	std::string sessionKey =  msessionKey;

	diet_profile_t* profile = diet_profile_alloc("sessionClose", 0, 0, 1); 
	//IN Parameters
	diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE);

	//OUT Parameters
	diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE);

	if(!diet_call(profile)) {
	    diet_string_get(diet_parameter(profile,1), &errorInfo, NULL);
	    std::cout << "errorInfo=" << errorInfo << std::endl;
	    merrorInfo = errorInfo;
	}
	
	res = (merrorInfo.size() > 0);
    
    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        merrorInfo += "\n";
        merrorInfo += ex.what();
       return 1;
    }

    return res;
  }
  
  UMS_Data::Session SessionProxy::getData() const
  {
    return msession;
  }

  std::string SessionProxy::getErrorInfo() const
  {
    return merrorInfo;
  }
 
  SessionProxy::~SessionProxy()
  {
  }
