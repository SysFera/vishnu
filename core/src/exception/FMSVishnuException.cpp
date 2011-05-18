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
/*
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_BATCH_SCHEDULER, std::string("The batch scheduler type is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_BATCH_SCHEDULER_ERROR, std::string("The batch scheduler indicates an error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_JOBID, std::string("The job id is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_PERMISSION_DENIED, std::string("Permission denied")));
  mp.insert(std::pair<int, std::string>(ERRCODE_ALREADY_TERMINATED, std::string("The job is already terminated")));
  mp.insert(std::pair<int, std::string>(ERRCODE_ALREADY_CANCELED, std::string("The job is already canceled")));
  mp.insert(std::pair<int, std::string>(ERRCODE_JOB_IS_NOT_TERMINATED, std::string("The job is not terminated")));
  
  */
}
