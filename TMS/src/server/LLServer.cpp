#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "LLServer.hpp"

LLServer::LLServer():BatchServer() {
    mserverName ='\0';
}

int LLServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job) {

  LL_job ll_job_info;
  options // TODO: A prendre en compte
  int llErrCode;
  if(llErrCode=llsubmit(scriptPath, NULL, NULL, &ll_job_info, LL_JOB_VERSION)) {
    return llErrCode ;//TODO: A remplacer par l'envoie d'une exception
  };

  ostringstream ll_job_id;
  llJobId<< (ll_job_info.step_list[0])->id.from_host;
  llJobId<< (ll_job_info.step_list[0])->id.cluster;
  llJobId<< (ll_job_info.step_list[0])->id.proc;
  
  job.setJobId(llJobId.str());
}

std::string LLServer::errorReport(int llErrCode) {

  switch (llErrCode) {
    case API_INVALID_INPUT:
      return "Invalid parameter.";
    case API_CANT_MALLOC:
      return "Out of memory."; 
    case API_CANT_CONNECT:
      return "Can't connect to CM."; 
    case API_CONFIG_ERR:
      return "Configuration error.";
    case API_CANT_TRANSMIT:
      return "Transaction problem."; 
    case API_CANT_AUTH:
      return "Not an administrator.";
    case API_CANT_FIND_PROC:
      return "No such step in CM.";    
    case API_WRNG_PROC_VERSION:
      return "Wrong proc version number.";   
    case API_WRNG_PROC_STATE:
      return "Wrong step state to start";
    case API_MACH_NOT_AVAIL:
      return "Machine not available";
    case API_CANT_FIND_RUNCLASS:
      return "Run class not available";
    case API_REQ_NOT_MET:
      return "Requirements not met";
    case API_WRNG_MACH_NO:
      return "Wrong machine number";
    case API_LL_SCH_ON:
      return "LL scheduler on";
    case API_MACH_DUP:
      return "Duplicated machine names";
    case API_NO_DCE_CRED:
      return "No dce credentials";
    case API_INSUFFICIENT_DCE_CRED:
      return "DCE credential lifetime less than 300 seconds";
    case API_64BIT_DCE_ERR:
      return "LoadLeveler 64-bit APIs not supported when DCE is enabled";
    case API_BAD_ADAPTER_USAGE:
      return "Bad Adapter Usage";
    case API_BAD_ADAPTER_DEVICE:
      return "Bad Adapter Device";
    case API_BAD_ADAPTER_USAGE_COUNT:
      return "Bad Adapter Usage Count";
    case API_BAD_ADAPTER_USAGE_PATTERN:
      return "Bad Adapter Usage Pattern";
    case API_BAD_PROTOCOL:
      return "Bad Adapter Usage Protocol";
    case API_INCOMPATIBLE_PROTOCOL:
      return "Incompatible Adapter Usage Protocols";
    case API_BAD_COMMUNICATION_SUBSYSTEM:
      return "Bad Adapter Usage Communication Subsystem";
    default:
      std::ostringstream os;
      os << "Unknown error," << llErrCode ; 
      return llErrCode.str();
  }
}
 
