#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"
#include "MachineProxy.h"
#include "LocalAccountProxy.h"
#include "ConfigurationProxy.h"
#include "OptionValueProxy.h"
#include "QueryProxy.h"

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

int restoreConfiguration(const std::string& sessionKey, const std::string filePath);

int configureOption(const std::string& sessionKey, const UMS_Data::OptionValue& optionValue);

int configureDefaultOption(const std::string& sessionKey, const UMS_Data::OptionValue& optionValue);

int listSessions(const std::string& sessionKey, UMS_Data::ListSessions& listSession, 
                 const UMS_Data::ListSessionOptions& options=UMS_Data::ListSessionOptions());

int listLocalAccount(const std::string& sessionKey, UMS_Data::ListLocalAccounts& listLocalAcc,
                     const UMS_Data::ListLocalAccOptions& options=UMS_Data::ListLocalAccOptions()); 

int listMachine(const std::string& sessionKey, UMS_Data::ListMachines& listMachine,
                const UMS_Data::ListMachineOptions& options=UMS_Data::ListMachineOptions());

int listHistoryCmd(const std::string& sessionKey, UMS_Data::ListCommands& listCommands,
                   const UMS_Data::ListCmdOptions& options=UMS_Data::ListCmdOptions());

int listOptions(const std::string& sessionKey, UMS_Data::ListOptionsValues& listOptValues,
                const UMS_Data::ListOptOptions& options=UMS_Data::ListOptOptions());

int listUsers(const std::string& sessionKey, UMS_Data::ListUsers& listUsers, 
              const std::string& userIdOption=std::string());
