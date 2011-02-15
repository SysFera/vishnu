#ifndef CLIUTILS_HH
#define CLIUTILS_HH

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>

#include "SessionProxy.h"
#include "MachineProxy.h"
#include "LocalAccountProxy.h"
#include "ConfigurationProxy.h"


std::string takePassword(const char*);

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


/*template<typename T, unsigned short int N>
class requiredParameter{

	public:

		requiredParameter():allProvided(false),provided(0){}

		void notify(const T& val){ this parameter is not used by the member function but it is here for convenience

			provided< N ? provided++:allProvided=true;
		}

		bool given()const{

			return allProvided;
		}

		~requiredParameter(){}

	private:
		unsigned short int provided;
		
		bool allProvided;
};*/












#endif
