#include "UserException.hh"


UserException::UserException(){
  mmsgc = "";
  mtype = NONE;
  mval = -1;
  initMsg();
}

UserException::UserException(const UserException& e){
  mmsgc = e.getMsgComp();
  mtype = e.getTypeI();
  mval  = e.getMsgI();
  initMsg();
}

UserException::UserException(int msg, std::string msgComp){
  mval = msg;
  mmsgc = msgComp;
  mtype = NONE;
  initMsg();
}

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
  std::map<int, std::string>::const_iterator it = mp.find(mval);
  if (it != mp.end())
    return it->second;
  return "Unknown error code for the User.\n";
}
