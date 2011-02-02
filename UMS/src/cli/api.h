#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"

int connect(std::string userId, std::string password, std::string& sessionKey, 
            UMS_Data::ConnectOptions connectOpt=UMS_Data::ConnectOptions()); 

int reconnect(std::string userId, std::string password, std::string sessionId, std::string& sessionKey);

int close(std::string sessionKey);
