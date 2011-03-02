/**
 * \file utils.cpp
 * \brief this file contains a definition of helper functions used by the command line interface 
 * \authors Daouda Traore (daouda.traore@sysfera.com) and Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */




#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <algorithm>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include "utils.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"
#include "LocalAccountProxy.hpp"
#include "ConfigurationProxy.hpp"
#include "Options.hpp"
using namespace std;



/**
 * \brief helper function to display help about a specific command 
 * \param opt: describes all options allowed by the command
 * \param signature: defines the usage of the command 
 */


void 
helpUsage (const Options& opt,const string& signature){

cout << "\nUsage: \n \n" << opt.getConfiguration()->getPgName()<<" " << signature <<"\n\n";

cout <<opt<< endl;
}


/**
 * \brief helper function to display error about a specific command 
 * \param cli   :The name of the command
 * \param errMsg: The error message to display 
 * \param err   : The error type 
 */


void 
errorUsage (const string & cli,const string& errMsg,const ErrorType& err){

		cerr << cli<<": "<<errMsg <<endl;

		if(err==PARAMERROR){
		cerr << "To get help, try <<"<< cli << " -h >>"<< endl;
		}
}


/**
 * \brief Helper function to display information (error or usage) about a specific command 
 * \param opt   :describes all options allowed by the command
 * \param mess: The help usage message to display 
 * \param eWhat   : The error message to display
 * \return 0 if an help is required or 1 if an error must me displayed
 */


int 
usage (const Options & opt,const std::string& mess,const std::string& eWhat){

	int res=0;

	if(opt.count("help")){

		helpUsage(opt,mess);
	}
	else{

		errorUsage(opt.getConfiguration()->getPgName(),eWhat);

		res=1;
	}

return res;
}



/**
 * \brief A helper function which check if VISHNU_CONFIG_FILE is set
 *\param opt: describes all options allowed by the command
 *\exception raise a runtime exception if the VISHNU_CONFIG_FILE is not set
 */


void 
checkVishnuConfig(const Options & opt){


if (opt.count("dietConfig")==0){


      throw runtime_error( "Set the VISHNU_CONFIG_FILE in your environment variable");

    }

}



/**
 * \brief Display a '-' caracter 
 * \param size: The number of '-' to diplay
 * \The output stream in which the display will be done.
 */

void
setFill(int size, ostream& os) {

  for(int i=0; i < size; i++) {
    os << "-";
  }
  os << "  ";

}


/**
 * \brief Helper function to display a session
 * \param os: An output stream in which the session will be printed 
 * \param session: The session to display
 * \return The output stream in which the session has been printed
 */

std::ostream&
operator<<(std::ostream& os, const UMS_Data::Session_ptr& session) {

  boost::posix_time::ptime pt;
  std::string undefined = "Undefined";
  std::string  dateLastConnectStr;
  std::string  dateCreateStr;
  std::string  dateCloseStr;

  std::string  sessionId = session->getSessionId();

  std::string  sessionKey = session->getSessionKey();


  std::string  userId = session->getUserId();

  long dateLastConnect = session->getDateLastConnect();
  if(dateLastConnect <= 0) {
    dateLastConnectStr = undefined;
  } else {
    pt =  boost::posix_time::from_time_t(dateLastConnect);
    dateLastConnectStr = boost::posix_time::to_simple_string(pt);
  } 

  long  dateCreate = session->getDateCreation();
  if(dateCreate <= 0) {
    dateCreateStr = undefined;
  } else {
    pt =  boost::posix_time::from_time_t(dateCreate);
    dateCreateStr = boost::posix_time::to_simple_string(pt);
  }

  long  dateClose = session->getDateClosure();
  if(dateClose <= 0) {
    dateCloseStr = undefined;
  } else {
    pt =  boost::posix_time::from_time_t(dateClose);
    dateCloseStr = boost::posix_time::to_simple_string(pt);
  }

  int  status = session->getStatus();
  std::string statusStr = (status?"ACTIVE":"INACTIVE");
  int  closePolicy = session->getClosePolicy();
  std::string closePolicyStr = (closePolicy==1?"CLOSE_ON_TIMEOUT":(closePolicy==2?"CLOSE_ON_DISCONNECT":"UNDEFINED"));
  int  timeOut = session->getTimeout();

  os << "============ Session for " << userId << "===========" << std::endl;
  os << setw(25) << right << "SessionKey: " << sessionKey << endl;
  os << setw(25) << right << "SessionId: " << sessionId << endl;
  os << setw(25) << right << "UserId: " << userId << endl;
  os << setw(25) << right << "DateLastConnect: "  << dateLastConnectStr << endl;
  os << setw(25) << right << "DateCreation: "  << dateCreateStr << endl ;
  os << setw(25) << right << "DateClosure: " << dateCloseStr << endl;
  os << setw(25) << right << "Status: " << status << " (" << statusStr << ")" << endl;
  os << setw(25) << right << "ClosePolicy: " << closePolicy << " (" << closePolicyStr << ")"  << endl;
  os << setw(25) << right << "TimeOut: " << timeOut << " seconds";
  os << endl;

 return os;

}


/**
 * \brief Helper function to display a list of session
 * \param os: The output stream in which the list will be printed 
 * \param listSession: The list to display
 * \return The output stream in which the list of sessions has been printed
 */




std::ostream&
operator<<(std::ostream& os, UMS_Data::ListSessions& listSession) {

  boost::posix_time::ptime pt;
  size_t maxSessionIdSize = std::string("SessionId").size(); 
  size_t maxUserIdSize = std::string("UserId").size();
  size_t maxDateLastConnectSize = std::string("DateLastConnect").size();
  size_t maxDateCreateSize = std::string("DateCreation").size();
  size_t maxDateCloseSize = std::string("DateClosure").size();
  size_t maxTimeOutSize = std::string("TimeOut").size();

  std::string blank = "  ----  ";
  std::string sessionId;
  std::string userId;
  std::string dateLastConnectStr = blank;
  std::string dateCreateStr = blank;
  std::string dateCloseStr = blank;
  long timeOut;
  long dateLastConnect;
  long dateCreate;
  long dateClose;
  int  status;
  int  closePolicy;

  for(int i = 0; i < listSession.getSessions().size(); i++) {

     sessionId = (listSession.getSessions().get(i))->getSessionId();
     maxSessionIdSize = max(maxSessionIdSize, sessionId.size());

     userId = (listSession.getSessions().get(i))->getUserId();
     maxUserIdSize = max(maxUserIdSize, userId.size());

     dateLastConnect = (listSession.getSessions().get(i))->getDateLastConnect();
     if(dateLastConnect > 0) {
       pt =  boost::posix_time::from_time_t(dateLastConnect);
       dateLastConnectStr = boost::posix_time::to_simple_string(pt);
     } 
     maxDateLastConnectSize = max(maxDateLastConnectSize, dateLastConnectStr.size());
     
     dateCreate = (listSession.getSessions().get(i))->getDateCreation();
     if(dateCreate > 0) {
       pt =  boost::posix_time::from_time_t(dateCreate);
       dateCreateStr = boost::posix_time::to_simple_string(pt);
     }
     maxDateCreateSize = max(maxDateCreateSize, dateCreateStr.size());

     dateClose = (listSession.getSessions().get(i))->getDateClosure();
     if(dateClose > 0) {
       pt =  boost::posix_time::from_time_t(dateClose);
       dateCloseStr = boost::posix_time::to_simple_string(pt);
     }
     maxDateCloseSize = max(maxDateCloseSize, dateCloseStr.size());
     
     timeOut = (listSession.getSessions().get(i))->getTimeout();
     ostringstream os_timeOut;
     os_timeOut << timeOut;
     maxTimeOutSize = max(maxTimeOutSize, (os_timeOut.str()).size());
  }

  os << setw(maxSessionIdSize+2) << left << "SessionId" << setw(maxUserIdSize+2) << left << "UserId" << setw(maxDateLastConnectSize+2) ;
  os << left << "DateLastConnect" << setw(maxDateCreateSize+2) << left << "DateCreation" << setw(maxDateCloseSize+2) << left << "DateClosure";
  os << setw(8) << left << "Status" << setw(13) << left << "ClosePolicy" << setw(maxTimeOutSize+2) << left << "TimeOut" << endl ;
  setFill(maxSessionIdSize, os);
  setFill(maxUserIdSize, os);
  setFill(maxDateLastConnectSize, os);
  setFill(maxDateCreateSize, os);
  setFill(maxDateCloseSize, os);
  setFill(6, os);
  setFill(11, os);
  setFill(maxTimeOutSize, os);
  os << endl;


  for(int i = 0; i < listSession.getSessions().size(); i++) {
    sessionId = (listSession.getSessions().get(i))->getSessionId();

    userId = (listSession.getSessions().get(i))->getUserId();

    
    dateLastConnect = (listSession.getSessions().get(i))->getDateLastConnect();
    if(dateLastConnect <= 0) {
      dateLastConnectStr = blank; 
    } else {
      pt =  boost::posix_time::from_time_t(dateLastConnect);
      dateLastConnectStr = boost::posix_time::to_simple_string(pt);
    }

    if(dateCreate <= 0) {
      dateCreateStr = blank;
    } else {
      dateCreate = (listSession.getSessions().get(i))->getDateCreation();
      pt =  boost::posix_time::from_time_t(dateCreate);
      dateCreateStr = boost::posix_time::to_simple_string(pt);
    }

    if(dateClose <= 0) {
      dateCloseStr = blank;
    } else {
      dateClose = (listSession.getSessions().get(i))->getDateClosure();
      pt =  boost::posix_time::from_time_t(dateClose);
      dateCloseStr = boost::posix_time::to_simple_string(pt);
    }

    status = (listSession.getSessions().get(i))->getStatus();

    closePolicy = (listSession.getSessions().get(i))->getClosePolicy();

    timeOut = (listSession.getSessions().get(i))->getTimeout();

    os << setw(maxSessionIdSize+2) << left << sessionId;
    os << setw(maxUserIdSize+2) << left << userId;
    os << setw(maxDateLastConnectSize+2) << left << dateLastConnectStr;
    os << setw(maxDateCreateSize+2) << left << dateCreateStr;
    os << setw(maxDateCloseSize+2) << left << dateCloseStr;
    os << setw(8) << left << status;
    os << setw(13) << left << closePolicy;
    os << setw(maxTimeOutSize) << left << timeOut;
    os << endl;

  }

 return os;


}


/**
 * \brief Helper function to display a local account
 * \param os: The output stream in which the local account will be printed 
 * \param account: The local account  to display
 * \return The output stream in which the local account has been printed
 */




ostream&
operator<<(ostream& os, const UMS_Data::LocalAccount_ptr& account) {

  std::string userId = account->getUserId();
  std::string machineId = account->getMachineId();
  std::string acLogin = account->getAcLogin();
  std::string sshKeyPath = account->getSshKeyPath();
  std::string homeDir = account->getHomeDirectory();

  os << "============ LocalAccount for " << acLogin << "===========" << std::endl;
  os << setw(25) << right << "UserId: " << userId << endl;
  os << setw(25) << right << "MachineId: " << machineId << endl;
  os << setw(25) << right << "AcLogin: "  << acLogin << endl;
  os << setw(25) << right << "SshKeyPath: "  << sshKeyPath << endl ;
  os << setw(25) << right << "HomeDirectory: " << homeDir;
  os << endl;

 return os;
}

/**
 * \brief Helper function to display a list of local accounts
 * \param os: The output stream in which the list will be printed 
 * \param lsLocalAccounts: The list to display
 * \return The output stream in which the list of local accounts has been printed
 */


ostream& operator<<(ostream& os,  UMS_Data::ListLocalAccounts& lsLocalAccount) {

  size_t maxUserSize = std::string("UserId").size();
  size_t maxMachineSize = std::string("MachineId").size();
  size_t maxAcLoginSize = std::string("AcLogin").size();
  std::string userId;
  std::string machineId;
  std::string acLogin;

  for(int i = 0; i < lsLocalAccount.getAccounts().size(); i++) {

     userId = (lsLocalAccount.getAccounts().get(i))->getUserId();
     maxUserSize = max(maxUserSize, userId.size());

     machineId = (lsLocalAccount.getAccounts().get(i))->getMachineId();
     maxMachineSize = max(maxMachineSize, machineId.size());

     acLogin = (lsLocalAccount.getAccounts().get(i))->getAcLogin();
     maxAcLoginSize = max(maxAcLoginSize, acLogin.size());
  }

  os << setw(maxUserSize+2) << left << "userId" << setw(maxMachineSize+2) << left << "machineId" << setw(maxAcLoginSize+2) << left << "acLogin";
  os << endl;
  setFill(maxUserSize, os);
  setFill(maxMachineSize, os);
  setFill(maxAcLoginSize, os);
  os << endl;


  for(int i = 0; i < lsLocalAccount.getAccounts().size(); i++) {
    userId = (lsLocalAccount.getAccounts().get(i))->getUserId();
    machineId = (lsLocalAccount.getAccounts().get(i))->getMachineId();
    acLogin = (lsLocalAccount.getAccounts().get(i))->getAcLogin();
    os << setw(maxUserSize+2) << left <<  userId;
    os << setw(maxMachineSize+2) << left << machineId;
    os << setw(maxAcLoginSize+2) << left << acLogin;
    os << endl;
  }

 return os;
}

/**
 * \brief Helper function to display a machine 
 * \param os: The output stream in which the machine will be printed 
 * \param machine: The machine  to display
 * \return The output stream in which the machine has been printed
 */



ostream&
 operator<<(ostream& os, const UMS_Data::Machine_ptr& machine) {

  std::string name = machine->getName();
  std::string machineId = machine->getMachineId();
  std::string site = machine->getSite();
  std::string descr = machine->getMachineDescription();
  std::string language = machine->getLanguage();
  int status = machine->getStatus();
  std::string statusStr = (status?"ACTIVE":"INACTIVE");

  os << "============ Machine for " << name << "===========" << std::endl;
  os << setw(25) << right << "Name: " << name << endl;
  os << setw(25) << right << "MachineId: " << machineId << endl;
  os << setw(25) << right << "Site: "  << site << endl;
  os << setw(25) << right << "Description: "  << descr << endl ;
  os << setw(25) << right << "Language: " << language << endl;
  os << setw(25) << right << "Status: " << status << " (" << statusStr << ")" << endl;


 return os;
}

/**
 * \brief Helper function to display a list of machines
 * \param os: The output stream in which the list will be printed 
 * \param lsMachine: The list to display
 * \return The output stream in which the list of machines has been printed
 */



std::ostream&
operator<<(std::ostream& os, UMS_Data::ListMachines& lsMachine) {

  size_t maxNameSize = std::string("Name").size();
  size_t maxMachineIdSize = std::string("machineId").size();
  size_t maxSiteSize = std::string("Site").size();
  std::string name;
  std::string machineId;
  std::string site;
  int  status;

  for(int i = 0; i < lsMachine.getMachines().size(); i++) {

     name = (lsMachine.getMachines().get(i))->getName();
     maxNameSize = max(maxNameSize, name.size());

     machineId = (lsMachine.getMachines().get(i))->getMachineId();
     maxMachineIdSize = max(maxMachineIdSize, machineId.size());

     site = (lsMachine.getMachines().get(i))->getSite();
     maxSiteSize = max(maxSiteSize, site.size());

  }

  os << setw(maxNameSize+2) << left << "Name" << setw(maxMachineIdSize+2) << left << "machineId" << setw(maxSiteSize+2) << left << "Site";
  os << setw(8) << left << "Status";
  os << endl;
  setFill(maxNameSize, os);
  setFill(maxMachineIdSize, os);
  setFill(maxSiteSize, os);
  setFill(6, os);
  os << endl;


  for(int i = 0; i < lsMachine.getMachines().size(); i++) {

     name = (lsMachine.getMachines().get(i))->getName();
     machineId = (lsMachine.getMachines().get(i))->getMachineId();
     site = (lsMachine.getMachines().get(i))->getSite();
     status = (lsMachine.getMachines().get(i))->getStatus();

     os << setw(maxNameSize+2) << left <<  name;
     os << setw(maxMachineIdSize+2) << left << machineId;
     os << setw(maxSiteSize+2) << left << site;
     os << setw(8) << left << status;
     os << endl;
  }

 return os;
}

/**
 * \brief Helper function to display a command 
 * \param os: The output stream in which the command will be printed 
 * \param command: The command  to display
 * \return The output stream in which the command has been printed
 */


std::ostream&
operator<<(std::ostream& os, const UMS_Data::Command_ptr& command) {

   std::string commandId = command->getCommandId();
   std::string sessionId = command->getSessionId();
   std::string machineId = command->getMachineId();
   std::string descr = command->getCmdDescription();

   boost::posix_time::ptime pt;
   std::string undefined = "Undefined";
   std::string startTimeStr = undefined;
   std::string endTimeStr = undefined;

   long startTime = command->getCmdStartTime();
   if(startTime > 0) {
     pt =  boost::posix_time::from_time_t(startTime);
     startTimeStr = boost::posix_time::to_simple_string(pt);
   }

   long endTime = command->getCmdEndTime();
   if(endTime > 0) {
     pt =  boost::posix_time::from_time_t(endTime);
     endTimeStr = boost::posix_time::to_simple_string(pt);
   }

   os << "============ Command for " << commandId << "===========" << std::endl;
   os << setw(25) << right << "CommandId: " << commandId << endl;
   os << setw(25) << right << "SessionId: " << sessionId << endl;
   os << setw(25) << right << "MachineId: "  << machineId << endl;
   os << setw(25) << right << "Description: "  << descr << endl ;
   os << setw(25) << right << "Start Time: " << startTimeStr << endl;
   os << setw(25) << right << "End Time: " << endTimeStr << endl;

 return os;
}

/**
 * \brief Helper function to display a list of command
 * \param os: The output stream in which the list will be printed 
 * \param lsCommand: The list to display
 * \return The output stream in which the list of command has been printed
 */



std::ostream&
operator<<(std::ostream& os, UMS_Data::ListCommands& lsCommand) {

   std::string commandId;
   std::string sessionId;
   std::string machineId;
   long startTime;
   long endTime;
   std::string blank =  "  ----  ";
   std::string startTimeStr = blank ;
   std::string endTimeStr = blank;
   size_t maxCommandIdSize = std::string("CommandId").size();
   size_t maxSessionIdSize = std::string("SessionId").size();
   size_t maxMachineIdSize = std::string("MachineId").size();
   size_t maxStartTimeSize = std::string("Start Time").size();
   size_t maxEndTimeSize = std::string("End Time").size();
   boost::posix_time::ptime pt;


   for(int i = 0; i < lsCommand.getCommands().size(); i++) {

     commandId = (lsCommand.getCommands().get(i))->getCommandId();
     maxCommandIdSize = max(maxCommandIdSize, commandId.size());


     sessionId = (lsCommand.getCommands().get(i))->getSessionId();
     maxSessionIdSize = max(maxSessionIdSize, sessionId.size());

     machineId = (lsCommand.getCommands().get(i))->getMachineId();
     maxMachineIdSize = max(maxMachineIdSize, machineId.size());

     startTime = (lsCommand.getCommands().get(i))->getCmdStartTime();
     if(startTime > 0) { 
       pt =  boost::posix_time::from_time_t(startTime);
       startTimeStr = boost::posix_time::to_simple_string(pt);
     }
     maxStartTimeSize = max(maxStartTimeSize, startTimeStr.size());
     
     endTime = (lsCommand.getCommands().get(i))->getCmdEndTime();
     if(endTime > 0) {
       pt =  boost::posix_time::from_time_t(endTime);
       endTimeStr = boost::posix_time::to_simple_string(pt);
     }
     maxEndTimeSize = max(maxEndTimeSize, endTimeStr.size());

  }

  os << setw(maxCommandIdSize+2) << left << "CommandId" << setw(maxSessionIdSize+2) << left << "SessionId" << setw(maxMachineIdSize+2) << left << "MachineId";
  os << setw(maxStartTimeSize+2) << left << "Start Time" << setw(maxEndTimeSize+2) << left << "End Time";
  os << endl;
  setFill(maxCommandIdSize, os);
  setFill(maxSessionIdSize, os);
  setFill(maxMachineIdSize, os);
  setFill(maxStartTimeSize, os);
  setFill(maxEndTimeSize, os);
  os << endl;

  for(int i = 0; i < lsCommand.getCommands().size(); i++) {

     commandId = (lsCommand.getCommands().get(i))->getCommandId();
     sessionId = (lsCommand.getCommands().get(i))->getSessionId();
     machineId = (lsCommand.getCommands().get(i))->getMachineId();

     startTime = (lsCommand.getCommands().get(i))->getCmdStartTime();
     if(startTime > 0) {
       pt =  boost::posix_time::from_time_t(startTime);
       startTimeStr = boost::posix_time::to_simple_string(pt);
     } else {
       startTimeStr = blank;
     }

     endTime = (lsCommand.getCommands().get(i))->getCmdEndTime();
     if(endTime > 0) {
       pt =  boost::posix_time::from_time_t(endTime);
       endTimeStr = boost::posix_time::to_simple_string(pt);
     } else {
       endTimeStr = blank;
     }

     os << setw(maxCommandIdSize+2) << left << commandId;
     os << setw(maxSessionIdSize+2) << left << sessionId;
     os << setw(maxMachineIdSize+2) << left << machineId;
     os << setw(maxStartTimeSize+2) << left << startTimeStr ;
     os << setw(maxEndTimeSize+2) << left << endTimeStr;
     os << endl;
  }

 return os;
}

/**
 * \brief Helper function to display an option value
 * \param os: The output stream in which the option value will be printed 
 * \param command: The option value to display
 * \return The output stream in which the option value has been printed
 */


std::ostream&
operator<<(std::ostream& os, const UMS_Data::OptionValue_ptr& optionValue) {

  std::string name = optionValue->getOptionName();
  std::string value = optionValue->getValue();

  os << "============ Option Value for " << name << "===========" << std::endl;
  os << setw(25) << right << "Name: " << name << endl;
  os << setw(25) << right << "Value: " << value << endl;

  return os;
}


/**
 * \brief Helper function to display a list of options
 * \param os: The output stream in which the list will be printed 
 * \param lsOptions: The list to display
 * \return The output stream in which the list of options has been printed
 */


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListOptionsValues& lsOptions) {

  std::string name;
  std::string value;
  size_t maxNameSize = std::string("Name").size();
  size_t maxValueSize = std::string("Value").size();

  for(int i = 0; i < lsOptions.getOptionValues().size(); i++) {

     name = (lsOptions.getOptionValues().get(i))->getOptionName();
     maxNameSize = max(maxNameSize, name.size());

     value = (lsOptions.getOptionValues().get(i))->getValue();
     maxValueSize = max(maxValueSize, value.size());

  }

  os << setw(maxNameSize+2) << left << "Name" << setw(maxValueSize+2) << left << "Value";
  os << endl;
  setFill(maxNameSize, os);
  setFill(maxValueSize, os);
  os << endl;

  for(int i = 0; i < lsOptions.getOptionValues().size(); i++) {

     name = (lsOptions.getOptionValues().get(i))->getOptionName();
     value = (lsOptions.getOptionValues().get(i))->getValue();

     os << setw(maxNameSize+2) << left <<  name;
     os << setw(maxValueSize+2) << left << value;
     os << endl;
  }

  return os;
}

/**
 * \brief Helper function to display a user
 * \param os: The output stream in which the user will be printed 
 * \param user: The user to display
 * \return The output stream in which the user has been printed
 */


std::ostream&
operator<<(std::ostream& os, const UMS_Data::User_ptr& user) {

  std::string firstName = user->getFirstname();
  std::string lastName = user->getLastname();
  int privilege = user->getPrivilege();
  std::string email = user->getEmail();
  std::string userId = user->getUserId();
  int status = user->getStatus();

  std::string privilegeStr = (privilege?"ADMIN":"USER");
  std::string statusStr = (status?"ACTIVE":"INACTIVE");

  os << "============ User for " << userId << "===========" << std::endl;
  os << setw(25) << right << "FirstName: " << firstName << endl;
  os << setw(25) << right << "LastName: " << lastName << endl;
  os << setw(25) << right << "Privilege: "  << privilege << " (" << privilegeStr << ")" << endl;
  os << setw(25) << right << "Status: "  << status << " (" << statusStr << ")" << endl;
  os << setw(25) << right << "Mail: "  << email << endl ;
  os << setw(25) << right << "UserId: " << userId << endl;

  return os;
}

/**
 * \brief Helper function to display a list of users
 * \param os: The output stream in which the list will be printed 
 * \param lsUsers: The list to display
 * \return The output stream in which the list of users has been printed
 */

std::ostream&
operator<<(std::ostream& os, UMS_Data::ListUsers& lsUsers) {

  std::string firstname;
  std::string lastname;
  int privilege;
  int status;
  std::string userId;
  size_t maxFirstnameSize = std::string("Firstname").size();
  size_t maxLastnameSize = std::string("Lastname").size();
  size_t maxUserIdSize = std::string("UserId").size();

  for(int i = 0; i < lsUsers.getUsers().size(); i++) {

     firstname = (lsUsers.getUsers().get(i))->getFirstname();
     maxFirstnameSize = max(maxFirstnameSize, firstname.size());

     lastname = (lsUsers.getUsers().get(i))->getLastname();
     maxLastnameSize = max(maxLastnameSize, lastname.size());

     userId = (lsUsers.getUsers().get(i))->getUserId();
     maxUserIdSize = max(maxUserIdSize, userId.size());
  }

  os << setw(maxFirstnameSize+2) << left << "Firstname" << setw(maxLastnameSize+2) << left << "Lastname" << setw(maxUserIdSize+2) << left << "UserId";
  os << setw(11) << left << "Privilege" << left << "Status";
  os << endl;
  setFill(maxFirstnameSize, os);
  setFill(maxLastnameSize, os);
  setFill(maxUserIdSize, os);
  setFill(9, os);
  setFill(6, os);
  os << endl;

  for(int i = 0; i < lsUsers.getUsers().size(); i++) {

     firstname = (lsUsers.getUsers().get(i))->getFirstname();
     lastname = (lsUsers.getUsers().get(i))->getLastname();
     userId = (lsUsers.getUsers().get(i))->getUserId();
     privilege = (lsUsers.getUsers().get(i))->getPrivilege();
     status = (lsUsers.getUsers().get(i))->getStatus();

     os << setw(maxFirstnameSize+2) << left << firstname;
     os << setw(maxLastnameSize+2) << left << lastname;
     os << setw(maxUserIdSize+2) << left << userId;
     os << setw(11) << left << privilege ;
     os << setw(8) << left << status ;
     os << endl;

  }


  return os;
}

