#include "UMSVishnuException.hpp"

UMSVishnuException::UMSVishnuException(UMSVishnuException& e):SystemException(e){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):SystemException(msg, msgComp){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException():SystemException(){
  mtype = VishnuException::UMS;
  initMsg();
}
void
UMSVishnuException::initMsg(){
  mp.insert(std::pair<int, std::string>(UNKNOWN_USER, std::string("The user is unknown")));
  mp.insert(std::pair<int, std::string>(INCORRECT_USERID, std::string("The userid is incorrect")));
  mp.insert(std::pair<int, std::string>(INCORRECT_PASSWORD, std::string("The password is incorrect")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_CLOSURE_MODE, std::string("The closure mode is unknown")));
  mp.insert(std::pair<int, std::string>(INCORRECT_TIMEOUT, std::string("The value of the timeout is incorrect")));
  mp.insert(std::pair<int, std::string>(UMS_SERVER_NOT_AVAILABLE, std::string("The server UMS is not available")));
  mp.insert(std::pair<int, std::string>(SESSIONKEY_NOT_FOUND, std::string("The session key is unrecognized")));
  mp.insert(std::pair<int, std::string>(SESSIONKEY_EXPIRED, std::string("The session key is expired. The session is closed.")));
  mp.insert(std::pair<int, std::string>(USERID_EXISTING, std::string("The userid already exists")));
  mp.insert(std::pair<int, std::string>(INVALID_MAIL_ADRESS, std::string("The mail adress is invalid")));
  mp.insert(std::pair<int, std::string>(SESSION_INCOMPATIBILITY, std::string("This session identifier is incompatible with the authenticated user for the given machine")));
  mp.insert(std::pair<int, std::string>(NO_ADMIN, std::string("The user is not an administrator")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_OPTION, std::string("the name of the option is unknown")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_SESSION_ID, std::string("The session id is unknown")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_MACHINE, std::string("The machine id is unknown")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_USERID, std::string("The userid is unknown")));
  mp.insert(std::pair<int, std::string>(LOCAL_ACCOUNT_EXIST, std::string("The local account already exists")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_LOCAL_ACCOUNT, std::string("The local is unknown")));
  mp.insert(std::pair<int, std::string>(COMMAND_RUNNING, std::string("Commands are running")));
  mp.insert(std::pair<int, std::string>(MACHINE_EXISTING, std::string("The machine id already exists")));
  mp.insert(std::pair<int, std::string>(INCORRECT_DATE_OPTION, std::string("The date option is incorrect")));
  mp.insert(std::pair<int, std::string>(SAVE_CONFIG_ERROR, std::string("A problem occurs during the configuration saving ")));
  mp.insert(std::pair<int, std::string>(RESTORE_CONFIG_ERROR, std::string("A problem occurs during the configuration restoring")));
  mp.insert(std::pair<int, std::string>(USERID_REQUIRED, std::string("The userid must be defined")));
  mp.insert(std::pair<int, std::string>(MACHINEID_REQUIRED, std::string("The machine id must be defined")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_SESSION_OPTION, std::string("The name of the session option is unknown")));
  mp.insert(std::pair<int, std::string>(UNKNOWN_MACHINE_FOR_SESSION, std::string("This session has been opened with another machine")));  
  mp.insert(std::pair<int, std::string>(TEMPORARY_PASSWORD, std::string("The password is a temporary password")));
  mp.insert(std::pair<int, std::string>(USER_LOCKED, std::string("The user is locked")));
  mp.insert(std::pair<int, std::string>(MACHINE_LOCKED, std::string("The machine is locked")));
  mp.insert(std::pair<int, std::string>(USER_ALREADY_LOCKED, std::string("The user is already locked")));
  mp.insert(std::pair<int, std::string>(MACHINE_ALREADY_LOCKED, std::string("The machine is already locked")));
  mp.insert(std::pair<int, std::string>(UNUSABLE_MACHINE, std::string("The machine does not exist or it is locked")));
  mp.insert(std::pair<int, std::string>(INCOMPATIBLE_USER_SESSION, std::string("The userid is incompatible with the session key")));
  
}