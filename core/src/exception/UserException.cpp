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
  mp.insert(std::pair<int, std::string>(4, std::string("Error bad parameters.")));
}

std::string
UserException::getMsg() const{
  if (mp.count(mval)){
    return mp[mval];
  }
  return "Unknown error code for the User.\n";
}
