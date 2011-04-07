

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
  if(options.getOutputPath().size()!=0) {
    set_attr(&attrib, ATTR_o, strdup(options.getOutputPath().c_str()));
  }
  if(options.getErrorPath().size()!=0) {
    set_attr(&attrib, ATTR_e, strdup(options.getErrorPath().c_str()));
  } 

  int connect = cnt2server(mserverName);
  std::cerr << "******* connect = " << connect << std::endl;
  if (connect <= 0) {
    std::cerr << "vishnu_job_submit: cannot connect to server ";
    std::cerr <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      std::cerr <<  "vishnu_job_submit: pbs_server daemon may not be running on host";
      std::cerr << pbs_server << "or hostname in file '$TORQUEHOME/server_name' may be incorrect" << std::endl;
    }

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
      std::cerr << "vishnu_job_submit:" << errmsg << std::endl;
    }
    else
    {
      std::cerr << "vishnu_job_submit: Error (" << pbs_errno << "-";
      std::cerr << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    return -1;
  }

  pbs_disconnect(connect);

  job.setJobId(std::string(jobId));

  return 0;
}

