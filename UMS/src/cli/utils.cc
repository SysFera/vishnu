#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>

#include "utils.hh"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"
#include "LocalAccountProxy.hpp"
#include "ConfigurationProxy.hpp"
using namespace std;
//namespace boost::posix_time = bp;

#define DATE_SIZE 25



std::string takePassword(const string& prompt, const string & salt){

string password=getpass(prompt.c_str());

string saltTmp="$5$"+salt+"$";

password=crypt(password.c_str(),saltTmp.c_str());

	return password.substr(saltTmp.size());

}


void setFill(int size, ostream& os) {

  for(int i=0; i < size; i++) {
    os << "-";
  }
  os << "  ";

}

std::ostream& operator<<(std::ostream& os, const UMS_Data::Session_ptr& session) {

  boost::posix_time::ptime pt;

  std::string  sessionId = session->getSessionId();

  std::string  sessionKey = session->getSessionKey();


  std::string  userId = session->getUserId();

  long dateLastConnect = session->getDateLastConnect();
  pt =  boost::posix_time::from_time_t(dateLastConnect);
  std::string  dateLastConnectStr = boost::posix_time::to_simple_string(pt);

  long  dateCreate = session->getDateCreation();
  pt =  boost::posix_time::from_time_t(dateCreate);
  std::string  dateCreateStr = boost::posix_time::to_simple_string(pt);

  long  dateClose = session->getDateClosure();
  pt =  boost::posix_time::from_time_t(dateClose);
  std::string  dateCloseStr = boost::posix_time::to_simple_string(pt);

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

std::ostream& operator<<(std::ostream& os, UMS_Data::ListSessions& listSession) {

  boost::posix_time::ptime pt;
  size_t maxSessionIdSize = 20;
  size_t maxUserIdSize = 20;
  size_t maxDateLastConnectSize = 20;
  size_t maxDateCreateSize = 20;
  size_t maxDateCloseSize = 20;
  size_t maxTimeOutSize = 7;

  std::string sessionId;
  std::string userId;
  std::string dateLastConnectStr;
  std::string dateCreateStr;
  std::string dateCloseStr;
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
     pt =  boost::posix_time::from_time_t(dateLastConnect);
     dateLastConnectStr = boost::posix_time::to_simple_string(pt);
     maxDateLastConnectSize = max(maxDateLastConnectSize, dateLastConnectStr.size());

     dateCreate = (listSession.getSessions().get(i))->getDateCreation();
     pt =  boost::posix_time::from_time_t(dateCreate);
     dateCreateStr = boost::posix_time::to_simple_string(pt);
     maxDateCreateSize = max(maxDateCreateSize, dateCreateStr.size());


     dateClose = (listSession.getSessions().get(i))->getDateClosure();
     pt =  boost::posix_time::from_time_t(dateClose);
     dateCloseStr = boost::posix_time::to_simple_string(pt);
     maxDateCloseSize = max(maxDateCloseSize, dateCloseStr.size());

     timeOut = (listSession.getSessions().get(i))->getTimeout();
     ostringstream os_timeOut;
     os_timeOut << timeOut;
     maxTimeOutSize = max(maxTimeOutSize, (os_timeOut.str()).size());
  }

  cout << setw(maxSessionIdSize+2) << left << "SessionId" << setw(maxUserIdSize+2) << left << "UserId" << setw(maxDateLastConnectSize+2) ;
  cout << left << "DateLastConnect" << setw(maxDateCreateSize+2) << left << "DateCreation" << setw(maxDateCloseSize+2) << left << "DateClosure";
  cout << setw(8) << left << "Status" << setw(13) << left << "ClosePolicy" << setw(maxTimeOutSize+2) << left << "TimeOut" << endl ;
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
    pt =  boost::posix_time::from_time_t(dateLastConnect);
    dateLastConnectStr = boost::posix_time::to_simple_string(pt);

    dateCreate = (listSession.getSessions().get(i))->getDateCreation();
    pt =  boost::posix_time::from_time_t(dateCreate);
    dateCreateStr = boost::posix_time::to_simple_string(pt);

    dateClose = (listSession.getSessions().get(i))->getDateClosure();
    pt =  boost::posix_time::from_time_t(dateClose);
    dateCloseStr = boost::posix_time::to_simple_string(pt);

    status = (listSession.getSessions().get(i))->getStatus();

    closePolicy = (listSession.getSessions().get(i))->getClosePolicy();

    timeOut = (listSession.getSessions().get(i))->getTimeout();

    cout << setw(maxSessionIdSize+2) << left << sessionId;
    cout << setw(maxUserIdSize+2) << left << userId;
    cout << setw(maxDateLastConnectSize+2) << left << dateLastConnectStr;
    cout << setw(maxDateCreateSize+2) << left << dateCreateStr;
    cout << setw(maxDateCloseSize+2) << left << dateCloseStr;
    cout << setw(8) << left << status;
    cout << setw(13) << left << closePolicy;
    cout << setw(maxTimeOutSize) << left << timeOut;
    cout << endl;

  }

 return os;


}

ostream& operator<<(ostream& os, const UMS_Data::LocalAccount_ptr& account) {

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

ostream& operator<<(ostream& os,  UMS_Data::ListLocalAccounts& lsLocalAccount) {

  size_t maxUserSize = 20;
  size_t maxMachineSize = 20;
  size_t maxAcLoginSize = 20;
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

  cout << setw(maxUserSize+2) << left << "userId" << setw(maxMachineSize+2) << left << "machineId" << setw(maxAcLoginSize+2) << left << "acLogin";
  cout << endl;
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

ostream& operator<<(ostream& os, const UMS_Data::Machine_ptr& machine) {

  std::string name = machine->getName();
  std::string machineId = machine->getMachineId();
  std::string site = machine->getSite();
  std::string descr = machine->getMachineDescription();
  std::string language = machine->getLanguage();
  int status = machine->getStatus();
  std::string statusStr = (status?"ACTIVE":"INACTIVE");

  os << "============ Machine for " << name << "===========" << std::endl;
  os << setw(25) << right << "UserId: " << name << endl;
  os << setw(25) << right << "MachineId: " << machineId << endl;
  os << setw(25) << right << "Site: "  << site << endl;
  os << setw(25) << right << "Description: "  << descr << endl ;
  os << setw(25) << right << "Language: " << language << endl;
  os << setw(25) << right << "Status: " << status << " (" << statusStr << ")" << endl;


 return os;
}

std::ostream& operator<<(std::ostream& os, UMS_Data::ListMachines& lsMachine) {

  size_t maxNameSize = 20;
  size_t maxMachineIdSize = 20;
  size_t maxSiteSize = 20;
  size_t maxDescrSisze = 20;
  size_t maxLanguageSize = 20;
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

  cout << setw(maxNameSize+2) << left << "Name" << setw(maxMachineIdSize+2) << left << "machineId" << setw(maxSiteSize+2) << left << "Site";
  cout << setw(8) << left << "Status";
  cout << endl;
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

std::ostream& operator<<(std::ostream& os, const UMS_Data::Command_ptr& command) {

   std::string commandId = command->getCommandId();
   std::string sessionId = command->getSessionId();
   std::string machineId = command->getMachineId();
   std::string descr = command->getCmdDescription();

   boost::posix_time::ptime pt;

   long startTime = command->getCmdStartTime();
   pt =  boost::posix_time::from_time_t(startTime);
   std::string  startTimeStr = boost::posix_time::to_simple_string(pt);

   long endTime = command->getCmdEndTime();
   pt =  boost::posix_time::from_time_t(endTime);
   std::string  endTimeStr = boost::posix_time::to_simple_string(pt);

   os << "============ Command for " << commandId << "===========" << std::endl;
   os << setw(25) << right << "CommandId: " << commandId << endl;
   os << setw(25) << right << "SessionId: " << sessionId << endl;
   os << setw(25) << right << "MachineId: "  << machineId << endl;
   os << setw(25) << right << "Description: "  << descr << endl ;
   os << setw(25) << right << "Start Time: " << startTimeStr << endl;
   os << setw(25) << right << "End Time: " << endTimeStr << endl;

 return os;
}

std::ostream& operator<<(std::ostream& os, UMS_Data::ListCommands& lsCommand) {

   std::string commandId;
   std::string sessionId;
   std::string machineId;
   long startTime;
   long endTime;
   std::string startTimeStr;
   std::string endTimeStr;
   size_t maxCommandIdSize = 20;
   size_t maxSessionIdSize = 20;
   size_t maxMachineIdSize = 20;
   size_t maxStartTimeSize = 20;
   size_t maxEndTimeSize = 20;
   boost::posix_time::ptime pt;


   for(int i = 0; i < lsCommand.getCommands().size(); i++) {

     commandId = (lsCommand.getCommands().get(i))->getCommandId();
     maxCommandIdSize = max(maxCommandIdSize, commandId.size());


     sessionId = (lsCommand.getCommands().get(i))->getSessionId();
     maxSessionIdSize = max(maxSessionIdSize, sessionId.size());

     machineId = (lsCommand.getCommands().get(i))->getMachineId();
     maxMachineIdSize = max(maxMachineIdSize, machineId.size());

     startTime = (lsCommand.getCommands().get(i))->getCmdStartTime();
     pt =  boost::posix_time::from_time_t(startTime);
     startTimeStr = boost::posix_time::to_simple_string(pt);
     maxStartTimeSize = (maxStartTimeSize, startTimeStr.size());

     endTime = (lsCommand.getCommands().get(i))->getCmdEndTime();
     pt =  boost::posix_time::from_time_t(endTime);
     endTimeStr = boost::posix_time::to_simple_string(pt);
     maxEndTimeSize = (maxEndTimeSize, endTimeStr.size());

  }

  cout << setw(maxCommandIdSize+2) << left << "CommandId" << setw(maxSessionIdSize+2) << left << "SessionId" << setw(maxMachineIdSize+2) << left << "MachineId";
  cout << setw(maxStartTimeSize+2) << left << "Start Time" << setw(maxEndTimeSize+2) << left << "End Time";
  cout << endl;
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
     pt =  boost::posix_time::from_time_t(startTime);
     startTimeStr = boost::posix_time::to_simple_string(pt);

     endTime = (lsCommand.getCommands().get(i))->getCmdEndTime();
     pt =  boost::posix_time::from_time_t(endTime);
     endTimeStr = boost::posix_time::to_simple_string(pt);

     os << setw(maxCommandIdSize+2) << left << commandId;
     os << setw(maxSessionIdSize+2) << left << sessionId;
     os << setw(maxMachineIdSize+2) << left << machineId;
     os << setw(maxStartTimeSize+2) << left << startTimeStr ;
     os << setw(maxEndTimeSize+2) << left << endTimeStr;
     os << endl;
  }

 return os;
}

std::ostream& operator<<(std::ostream& os, const UMS_Data::OptionValue_ptr& optionValue) {

  std::string name = optionValue->getOptionName();
  std::string value = optionValue->getValue();

  os << "============ Option Value for " << name << "===========" << std::endl;
  os << setw(25) << right << "Name: " << name << endl;
  os << setw(25) << right << "Value: " << value << endl;

  return os;
}

std::ostream& operator<<(std::ostream& os, UMS_Data::ListOptionsValues& lsOptions) {

  std::string name;
  std::string value;
  size_t maxNameSize = 20;
  size_t maxValueSize = 20;

  for(int i = 0; i < lsOptions.getOptionValues().size(); i++) {

     name = (lsOptions.getOptionValues().get(i))->getOptionName();
     maxNameSize = max(maxNameSize, name.size());

     value = (lsOptions.getOptionValues().get(i))->getValue();
     maxValueSize = max(maxValueSize, value.size());

  }

  cout << setw(maxNameSize+2) << left << "Name" << setw(maxValueSize+2) << left << "Value";
  cout << endl;
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

std::ostream& operator<<(std::ostream& os, const UMS_Data::User_ptr& user) {

  std::string firstName = user->getFirstname();
  std::string lastName = user->getLastname();
  int privilege = user->getPrivilege();
  std::string email = user->getEmail();
  std::string userId = user->getUserId();

  std::string privilegeStr = (privilege?"ADMIN":"USER");

  os << "============ User for " << userId << "===========" << std::endl;
  os << setw(25) << right << "FirstName: " << firstName << endl;
  os << setw(25) << right << "LastName: " << lastName << endl;
  os << setw(25) << right << "Privilege: "  << privilege << " (" << privilegeStr << ")" << endl;
  os << setw(25) << right << "Mail: "  << email << endl ;
  os << setw(25) << right << "UserId: " << userId << endl;

  return os;
}

std::ostream& operator<<(std::ostream& os, UMS_Data::ListUsers& lsUsers) {

  std::string firstname;
  std::string lastname;
  int privilege;
  std::string userId;
  size_t maxFirstnameSize = 20;
  size_t maxLastnameSize = 20;
  size_t maxUserIdSize = 20;

  for(int i = 0; i < lsUsers.getUsers().size(); i++) {

     firstname = (lsUsers.getUsers().get(i))->getFirstname();
     maxFirstnameSize = max(maxFirstnameSize, firstname.size());

     lastname = (lsUsers.getUsers().get(i))->getLastname();
     maxLastnameSize = max(maxLastnameSize, lastname.size());

     userId = (lsUsers.getUsers().get(i))->getUserId();
     maxUserIdSize = max(maxUserIdSize, userId.size());
  }

  cout << setw(maxFirstnameSize+2) << left << "Firstname" << setw(maxLastnameSize+2) << left << "Lastname" << setw(maxUserIdSize+2) << left << "UserId";
  cout << setw(11) << left << "Privilege";
  cout << endl;
  setFill(maxFirstnameSize, os);
  setFill(maxLastnameSize, os);
  setFill(maxUserIdSize, os);
  setFill(9, os);
  os << endl;

  for(int i = 0; i < lsUsers.getUsers().size(); i++) {

     firstname = (lsUsers.getUsers().get(i))->getFirstname();
     lastname = (lsUsers.getUsers().get(i))->getLastname();
     userId = (lsUsers.getUsers().get(i))->getUserId();
     privilege = (lsUsers.getUsers().get(i))->getPrivilege();

     os << setw(maxFirstnameSize+2) << left << firstname;
     os << setw(maxLastnameSize+2) << left << lastname;
     os << setw(maxUserIdSize+2) << left << userId;
     os << setw(11) << left << privilege ;
     os << endl;

  }


  return os;
}

