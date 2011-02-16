#ifndef CLIUTILS_HH
#define CLIUTILS_HH

#include "UMS_Data.hpp"
#include <iostream>


std::string takePassword(const std::string&, const std::string&);

std::ostream& operator<<(std::ostream& os, const UMS_Data::Session_ptr& session);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListSessions& listSession);

std::ostream& operator<<(std::ostream& os, const UMS_Data::LocalAccount_ptr& account);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListLocalAccounts& lsLocalAccounts);

std::ostream& operator<<(std::ostream& os, const UMS_Data::Machine_ptr& machine);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListMachines& lsMachine);

std::ostream& operator<<(std::ostream& os, const UMS_Data::Command_ptr& command);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListCommands& lsCommand);

std::ostream& operator<<(std::ostream& os,const UMS_Data::OptionValue_ptr& optionValue);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListOptionsValues& lsOptions);

std::ostream& operator<<(std::ostream& os, const UMS_Data::User_ptr& user);
std::ostream& operator<<(std::ostream& os, UMS_Data::ListUsers& lsUsers);



#endif
