#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <algorithm>

#include "utils.hh"

using namespace std;
//namespace boost::posix_time = bp;

#define DATE_SIZE 25



 std::string takePassword(const char* prompt){return std::string(getpass(prompt));}


void setFill(int size, ostream& os) {

  for(int i=0; i < size; i++) {
    os << "-";
  }
  os << "  ";

}

std::ostream& operator<<(std::ostream& os, const UMS_Data::Session_ptr& session) {


}

std::ostream& operator<<(std::ostream& os, UMS_Data::ListSessions& listSession) {

  boost::posix_time::ptime pt;
  size_t maxSessionIdSize = 20;
  size_t maxUserIdSize = 20;
  size_t maxDateLastConnectSize = 20;
  size_t maxDateCreateSize = 20;
  size_t maxDateCloseSize = 20;
  size_t maxTimeOutSize = 5;
 
  std::string sessionId;
  std::string userId;
  std::string dateLastConnectStr;
  std::string dateCreateStr;
  std::string dateCloseStr;
  long timeOut; 
  ostringstream os_timeOut;
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
    maxDateCreateSize = ((maxDateCreateSize < dateCreateStr.size())?dateCreateStr.size():maxDateCreateSize);

    dateClose = (listSession.getSessions().get(i))->getDateClosure();
    pt =  boost::posix_time::from_time_t(dateClose);
    dateCloseStr = boost::posix_time::to_simple_string(pt);
    maxDateCloseSize = ((maxDateCloseSize < dateCloseStr.size())?dateCloseStr.size():maxDateCloseSize);

    status = (listSession.getSessions().get(i))->getStatus();
    //os_status << status; 
  
    closePolicy = (listSession.getSessions().get(i))->getClosePolicy();
    //os_closePolicy << closePolicy ;

    timeOut = (listSession.getSessions().get(i))->getTimeout(); 
    //os_timeOut << timeOut;

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

void printSession(UMS_Data::Session_ptr session) {

  ostringstream os_dc;
  ostringstream os_dcl;
  ostringstream os_s;
  ostringstream os_c;
  ostringstream os_t;

  char* date_dlc;
  char* date_dc = (char*) malloc(DATE_SIZE*sizeof(char));
  char* date_dcl = (char*) malloc(DATE_SIZE*sizeof(char));
  boost::posix_time::ptime pt;
  struct tm *date_tm;


  std::string sessionId = session->getSessionId();
  std::string userId = session->getUserId();
  std::string sessionKey = session->getSessionKey();
  long dateLastConnect = session->getDateLastConnect();
  
  //date_tm = localtime(&dateLastConnect); strftime(date_dlc, DATE_SIZE, "%m/%d %H:%M", date_tm);
  pt =  boost::posix_time::from_time_t(dateLastConnect); date_dlc = strdup((boost::posix_time::to_simple_string(pt).c_str()));
  long dateCreation = session->getDateCreation();
  date_tm = localtime(&dateCreation); strftime(date_dc, DATE_SIZE, "%m/%d %H:%M", date_tm);
  long dateClosure = session->getDateClosure();
  date_tm = localtime(&dateClosure); strftime(date_dcl, DATE_SIZE, "%m/%d %H:%M", date_tm);
  int state = session->getStatus(); os_s << state;
  int closePolicy = session->getClosePolicy(); os_c << closePolicy ;
  long timeOut = session->getTimeout(); os_t << timeOut;


  printf("%-12s %-12s %-15s %-25s %-15s %-15s %-2s %-11s %-7s\n",
  sessionId.c_str(), userId.c_str(), sessionKey.c_str(), date_dlc, date_dc, date_dcl,
    (os_s.str()).c_str(), (os_c.str()).c_str(), (os_t.str()).c_str());

}

ostream& operator<<(ostream& os, const UMS_Data::LocalAccount_ptr& account) {

  std::string userId = account->getUserId();
  std::string machineId = account->getMachineId();
  std::string acLogin = account->getAcLogin();
  std::string sshKeyPath = account->getSshKeyPath();
  std::string homeDir = account->getHomeDirectory();

  os << "============ LocalAccount for " << acLogin << "===========" << std::endl;
  os << setw(25) << "userId: " << userId << endl;
  os << setw(25) << "machineId: " << machineId << endl; 
  os << setw(25) << "acLogin: "  << left << acLogin << endl;
  os << setw(25) << right << "sshKeyPath: "  << sshKeyPath << endl ;
  os << setw(25) << right << "homeDirectory: " << homeDir << endl; 
 
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

void printMachine(UMS_Data::Machine_ptr machine) {

  std::string name = machine->getName();
  std::string machineId = machine->getMachineId();
  std::string site = machine->getSite();
  std::string descr = machine->getMachineDescription();
  std::string language = machine->getLanguage();

  printf("%-15s %-15s %-15s %-20s %-15s\n", machineId.c_str(), name.c_str(), site.c_str(), descr.c_str(), language.c_str());
}

void printCommand(UMS_Data::Command_ptr command) {

  std::string commandId = command->getCommandId();
  std::string sessionId = command->getSessionId();
  std::string machineId = command->getMachineId();
  std::string descr = command->getCmdDescription();

  char* date_startTime = (char*) malloc(DATE_SIZE*sizeof(char));
  char* date_endTime = (char*) malloc(DATE_SIZE*sizeof(char));
  struct tm *date_tm;

  long  startTime = command->getCmdStartTime();
  date_tm = localtime(&startTime); strftime(date_startTime, DATE_SIZE, "%m/%d %H:%M", date_tm);
  long  endTime = command->getCmdEndTime();
  date_tm = localtime(&endTime); strftime(date_endTime, DATE_SIZE, "%m/%d %H:%M", date_tm);

  printf("%-15s %-15s %-15s %-20s %-15s %-15s\n", commandId.c_str(), sessionId.c_str(), machineId.c_str(), descr.c_str(), date_startTime, date_endTime);

}

void printOptionValue(UMS_Data::OptionValue_ptr optionValue) {

  std::string name = optionValue->getOptionName();
  std::string value = optionValue->getValue();

  printf("%-15s %-15s\n", name.c_str(), value.c_str());
}

void printUser(UMS_Data::User_ptr user) {

  ostringstream os_privilege;

  std::string firstName = user->getFirstname();
  std::string lastName = user->getLastname();
  int privilege = user->getPrivilege(); os_privilege << privilege;
  std::string email = user->getEmail();
  std::string userId = user->getUserId();

  printf("%-15s %-15s %-9s %-30s %-15s\n", firstName.c_str(), lastName.c_str(), (os_privilege.str()).c_str(), email.c_str(), userId.c_str());
}


void print(UMS_Data::User_ptr user, int id) {

  cout << "============ User " << id << " Content ===========" << endl;
  cout << "    firstname=" << user->getFirstname() << endl;
  cout << "    lastname=" << user->getLastname() << endl;
  cout << "    privilege=" << user->getPrivilege() << endl;
  cout << "    mail=" << user->getEmail() << endl;
  cout << "    userId=" << user->getUserId() << endl;
  cout << "=======================================" << endl;
}

void print(UMS_Data::Machine_ptr machine, int id) {

  cout << "============ machine " << id << " Content ===========" << endl;
  cout << "    machineId=" << machine->getMachineId() << endl;
  cout << "    name=" << machine->getName() << endl;
  cout << "    site=" << machine->getSite() << endl;
  cout << "    machineDescription=" << machine->getMachineDescription() << endl;
  cout << "    language=" << machine->getLanguage() << endl;
  cout << "=======================================" << endl;
}

void  print(UMS_Data::LocalAccount_ptr localAccount, int id) {

  cout << "============ localAccount " << id << " Content ===========" << endl;
  cout << "    UserId=" << localAccount->getUserId() << endl;
  cout << "    machineId=" << localAccount->getMachineId() << endl;
  cout << "    acLogin=" << localAccount->getAcLogin() << endl;
  cout << "    sshKeyPath=" << localAccount->getSshKeyPath() << endl;
  cout << "    homeDirectory=" << localAccount->getHomeDirectory() << endl;
  cout << "=======================================" << endl;

}
