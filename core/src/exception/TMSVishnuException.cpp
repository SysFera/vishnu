#include "TMSVishnuException.hpp"


TMSVishnuException::TMSVishnuException(const TMSVishnuException& e):UserException(e){
  mtype = VishnuException::TMS;
  initMsg();
}

TMSVishnuException::TMSVishnuException(int msg, std::string msgComp):UserException(msg, msgComp){
  mtype = VishnuException::TMS;
  initMsg();
}

TMSVishnuException::TMSVishnuException():UserException(){
  mtype = VishnuException::TMS;
  initMsg();
}

void
TMSVishnuException::initMsg(){

  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_BATCH_SCHEDULER, std::string("The batch scheduler type is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_BATCH_SCHEDULER_ERROR, std::string("The batch scheduler indicates an error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_JOBID, std::string("The job id is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_CANT_CANCEL, std::string("Can not cancel this job")));
  mp.insert(std::pair<int, std::string>(ERRCODE_TMS_NOT_ADMIN, std::string("The user is not an administrator")));

}
