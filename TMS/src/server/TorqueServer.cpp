

#include "TorqueServer.hpp"

TorqueServer::TorqueServer():BatchServer() {
  mserverName ='\0';
}

int TorqueServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job) {

  char* destination = '\0';
  struct attrl *attrib= NULL;

  if(options.getName().size()!=0){
    set_attr(&attrib, (char*)ATTR_N, strdup(options.getName().c_str()));
  } else {
    set_attr(&attrib, (char*)ATTR_N, (char*)"defaut_name");
  }
  if(options.getQueue().size()!=0) {
     destination = strdup(options.getQueue().c_str());  
  }
  /*if(options.getOutputPath().size()!=0) {
    set_attr(&attrib, ATTR_e, Job.getOutputath());
  }
  if(options.getErrorPath().size()!=0) {
    set_attr(&attrib, ATTR_e, Job.getErrorPath());
  }*/ 

  int connect = cnt2server(mserverName);
  std::cerr << "******* connect = " << connect << std::endl;
  if (connect <= 0) {
    return -1;
  }
  std::cout << "  scriptPath=" << scriptPath << std::endl;
  char*jobId = pbs_submit(connect, (struct attropl *)attrib,
                          const_cast<char*>(scriptPath), destination, NULL);

  if (jobId == NULL) {
    std::cerr << "******* jobId is NULL ...." << std::endl;
    char* errmsg = pbs_geterrmsg(connect);

    if (errmsg != NULL)
    {
      std::cerr << "tms_submit:" << errmsg << std::endl;
    }
    else
    {
      std::cerr << "tms_submit: Error (" << pbs_errno << "-";
      std::cerr << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    return -1;
  }

  job.setJobId(std::string(jobId));

  return 0;
}

