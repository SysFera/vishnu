 #include <string>
 #include <vector>
 #include <list>
 #include <iostream>
 #include <assert.h>
 #include <string.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <fstream>

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
    std::string sshKey1;
    std::string sshKey2;
    std::string sshKey3;
    std::string optionsToString;
    std::string sessionId ;
    char* sessionkey;
    char* errorInfo;
    size_t length;
    char* key;
    char* encryptedKey;
    std::string msg = "call of function diet_string_set is rejected ";

    sshKey1 = "/etc/ssh/ssh_host_dsa_key.pub";
    sshKey2 = "/etc/ssh/ssh_host_rsa_key.pub";
    sshKey3 = std::string(getenv("HOME"))+"/.vishnu";
    gethostname(hostname, HOST_NAME_MAX_SIZE);  

    std::ifstream ifile;
    std::ifstream ifile1(sshKey1.c_str());
    std::ifstream ifile2(sshKey2.c_str());
    std::ifstream ifile3(sshKey3.c_str());
    
    bool checkFile1 = ifile1.is_open();
    bool checkFile2 = ifile2.is_open();
    bool checkFile3 = ifile3.is_open(); 
     
    if(!checkFile1 && !checkFile2 && !checkFile3) {
         throw std::runtime_error("can't open file "+sshKey1+" or "+sshKey2+" or "+sshKey3+":\n"+
                                  "You must copy the file of your sshKey in one of three free files:\n"
                                  +"/etc/ssh/ssh_host_dsa_key.pub, or\n"
                                  +"/etc/ssh/ssh_host_rsa_key.pub, or\n"
                                  +"$HOME/.vishnu\n"
                                  );
    } 
      
    if(checkFile1) {
        ifile1.close();
        ifile.open(sshKey1.c_str());
       if(checkFile2) {
         ifile2.close();
       }
       if(checkFile3) {
         ifile3.close();
       }
    }
    else if(checkFile2) {
           ifile2.close();
           ifile.open(sshKey2.c_str());
           if(checkFile3) {
             ifile3.close();
           }
    } 
    else {
        ifile3.close();
        ifile.open(sshKey3.c_str());
    }

    ifile.seekg(0, std::ios::end);
      length = ifile.tellg();
    ifile.seekg(0, std::ios::beg);
 
    key = new char[length];
    ifile.read(key, length);
    ifile.close();

    std::string salt = "$6$"+user.getData().getUserId()+"$";                 
    encryptedKey = crypt(key, salt.c_str());

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
    if(diet_string_set(diet_parameter(profile,2), encryptedKey+salt.length(), DIET_VOLATILE)) {
       msg += "with sshKey parameter sshKey path";
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

      if(key!=NULL) delete [] key;

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
