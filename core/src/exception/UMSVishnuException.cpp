#include "UMSVishnuException.hpp"

UMSVishnuException::UMSVishnuException(const UMSVishnuException& e):UserException(e){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::UMS;
  initMsg();
}
UMSVishnuException::UMSVishnuException():UserException(){
  mtype = VishnuException::UMS;
  initMsg();
}
void
UMSVishnuException::initMsg(){
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_USER, std::string("The user is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_USERID, std::string("The userid is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_USERID_EXISTING, std::string("The userid already exists")));
  mp.insert(std::pair<int, std::string>(ERRCODE_USER_LOCKED, std::string("The user is locked")));
  mp.insert(std::pair<int, std::string>(ERRCODE_USER_ALREADY_LOCKED, std::string("The user is already locked")));
  mp.insert(std::pair<int, std::string>(ERRCODE_NO_ADMIN, std::string("The user is not an administrator")));
  mp.insert(std::pair<int, std::string>(ERRCODE_TEMPORARY_PASSWORD, std::string("The password is a temporary password")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INVALID_MAIL_ADRESS, std::string("The mail adress is invalid")));

  mp.insert(std::pair<int, std::string>(ERRCODE_SESSIONKEY_NOT_FOUND, std::string("The session key is unrecognized")));
  mp.insert(std::pair<int, std::string>(ERRCODE_SESSIONKEY_EXPIRED, std::string("The session key is expired. The session is closed.")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_SESSION_ID, std::string("The session id is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_COMMAND_RUNNING, std::string("Commands are running")));

  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_MACHINE, std::string("The machine id is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_MACHINE_EXISTING, std::string("The machine id already exists")));
  mp.insert(std::pair<int, std::string>(ERRCODE_MACHINE_LOCKED, std::string("The machine is locked")));
  mp.insert(std::pair<int, std::string>(ERRCODE_MACHINE_ALREADY_LOCKED, std::string("The machine is already locked")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNUSABLE_MACHINE, std::string("The machine does not exist or it is locked")));

  mp.insert(std::pair<int, std::string>(ERRCODE_LOCAL_ACCOUNT_EXIST, std::string("The local account already exists")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, std::string("The local is unknown")));

  mp.insert(std::pair<int, std::string>(ERRCODE_SAVE_CONFIG_ERROR, std::string("A problem occurs during the configuration saving ")));
  mp.insert(std::pair<int, std::string>(ERRCODE_RESTORE_CONFIG_ERROR, std::string("A problem occurs during the configuration restoring")));

  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_OPTION, std::string("The name of the user option is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_CLOSURE_MODE, std::string("The closure policy is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_INCORRECT_TIMEOUT, std::string("The value of the timeout is incorrect")));

  mp.insert(std::pair<int, std::string>(ERRCODE_ROOT_USER_ONLY, std::string("It can be done by the root user only")));
}
