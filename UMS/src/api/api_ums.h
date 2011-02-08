#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"
#include "MachineProxy.h"
#include "LocalAccountProxy.h"
#include "ConfigurationProxy.h"

void print(UMS_Data::User_ptr user, int id);
void print(UMS_Data::Machine_ptr machine, int id); 
void  print(UMS_Data::LocalAccount_ptr localAccount, int id);

int vishnuInitialize(char* cfg, int argc, char** argv);

void vishnuFinalize();

int connect(const std::string& userId, const std::string& password, std::string& sessionKey, 
            const UMS_Data::ConnectOptions& connectOpt=UMS_Data::ConnectOptions()); 

int reconnect(const std::string& userId, const std::string& password, const std::string& sessionId, std::string& sessionKey);

int close(const std::string& sessionKey);

int addVishnuUser(const std::string& sessionKey, const UMS_Data::User& newUser);

int updateUser(const std::string& sessionKey, const UMS_Data::User& user);

int deleteUser(const std::string& sessionKey, const std::string& userId);

int changePassword(const std::string& userId, const std::string& password, const std::string& passwordNew);

int resetPassword(const std::string& sessionKey, const std::string& userId);

int addMachine(const std::string& sessionKey, const UMS_Data::Machine& newMachine);

int updateMachine(const std::string& sessionKey, const UMS_Data::Machine& machine);

int deleteMachine(const std::string& sessionKey, const std::string& machineId);

int addLocalAccount(const std::string& sessionKey, const UMS_Data::LocalAccount& newLocalAccount);

int updateLocalAccount(const std::string& sessionKey, const UMS_Data::LocalAccount& localAccount);

int deleteLocalAccount(const std::string& sessionKey, const std::string& userId, const std::string& machineId);

int saveConfiguration(const std::string& sessionKey, const std::string filePath, UMS_Data::Configuration& config);

int restoreConfiguration(const std::string& sessionKey, const UMS_Data::Configuration& config);
