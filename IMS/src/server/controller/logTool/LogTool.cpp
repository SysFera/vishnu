#include "LogTool.hpp"
#include <unistd.h>

LogTool::LogTool(){
  char host[HOSTSIZE];
  if (gethostname(host, HOSTSIZE)){
    mname = "IMSLogT";
  }
  else{
    mname = "IMSLogT_";
    mname += string(host);
  }
}

LogTool::~LogTool(){
}
