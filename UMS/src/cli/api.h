#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"

int connect(const std::string& userId, const std::string& password, std::string& sessionKey, 
            const UMS_Data::ConnectOptions& connectOpt=UMS_Data::ConnectOptions()); 

int reconnect(const std::string& userId, const std::string& password, const std::string& sessionId, std::string& sessionKey);

int close(const std::string& sessionKey);
