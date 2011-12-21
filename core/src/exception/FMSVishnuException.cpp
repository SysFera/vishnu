#include "FMSVishnuException.hpp"


FMSVishnuException::FMSVishnuException(const FMSVishnuException& e):UserException(e){
  mtype = VishnuException::FMS;
  initMsg();
}

FMSVishnuException::FMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::FMS;
  initMsg();
}

FMSVishnuException::FMSVishnuException():UserException(){
  mtype = VishnuException::FMS;
  initMsg();
}

void
FMSVishnuException::initMsg(){

  mp.insert(std::pair<int, std::string>(ERRCODE_INVALID_PATH, std::string("Invalid path ")));
  mp.insert(std::pair<int, std::string>(ERRCODE_RUNTIME_ERROR, std::string("Remote command execution error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_FILETRANSFERID, std::string("The file transfer  id is unknown")));
  
}
