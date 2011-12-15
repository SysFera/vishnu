#include "UserException.hpp"


UserException::UserException():VishnuException(){ initMsg();}

UserException::UserException(const UserException& e):VishnuException(e){ initMsg();}

UserException::UserException(int msg, std::string msgComp):VishnuException(msg,msgComp){ initMsg();}

std::string
UserException::getTypeS() const{
  return std::string("User");
}

void
UserException::initMsg(){
  mp.insert(std::pair<int, std::string>(ERRCODE_INVALID_PARAM, std::string("Error invalid parameters")));
  mp.insert(std::pair<int, std::string>(ERRCODE_FILENOTFOUND, std::string("File not found")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CONFIGNOTFOUND, std::string("Undefined configuration parameter")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CLI_ERROR_NO_SESSION, std::string("Session key not found")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CLI_ERROR_MISSING_PARAMETER, std::string("Missing parameters")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CLI_ERROR_DIET, std::string("Vishnu initialization failed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CLI_ERROR_RUNTIME, std::string("Undefined error")));
}

std::string
UserException::getMsg() const{
  if (mp.count(mval)){
    return mp[mval];
  }
  return "Unknown error code for the User.\n";
}
