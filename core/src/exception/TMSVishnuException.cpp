#include "TMSVishnuException.hpp"

#include <map>                          // for map
#include <utility>                      // for pair

#include "VishnuException.hpp"          // for VishnuException, etc

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

  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_BATCH_SCHEDULER, std::string("Unknown batch")));
  mp.insert(std::pair<int, std::string>(ERRCODE_BATCH_SCHEDULER_ERROR, std::string("Batch error")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_JOBID, std::string("The job id is unknown")));
  mp.insert(std::pair<int, std::string>(ERRCODE_PERMISSION_DENIED, std::string("Permission denied")));
  mp.insert(std::pair<int, std::string>(ERRCODE_ALREADY_TERMINATED, std::string("The job is already terminated")));
  mp.insert(std::pair<int, std::string>(ERRCODE_ALREADY_CANCELED, std::string("The job is already canceled")));
  mp.insert(std::pair<int, std::string>(ERRCODE_JOB_IS_NOT_TERMINATED, std::string("The job is not completed")));
  mp.insert(std::pair<int, std::string>(ERRCODE_ALREADY_DOWNLOADED, std::string("The job is already downloaded")));
  mp.insert(std::pair<int, std::string>(ERRCODE_UNKNOWN_WORKID, std::string("The work id is unknown")));
}
