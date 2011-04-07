

#include "TorqueServer.hpp"
#include <sstream>

TorqueServer::TorqueServer():BatchServer() {
  mserverName ='\0';
}

int TorqueServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job) {

  char* destination = '\0';
  struct attrl *attrib= NULL;
  std::ostringstream os_str;

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
    os_str << "vishnu_job_submit: cannot connect to server ";
    os_str <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      os_str <<  "vishnu_job_submit: pbs_server daemon may not be running on host";
      os_str << pbs_server << "or hostname in file '$TORQUEHOME/server_name' may be incorrect" << std::endl;
    }

    std::cout << os_str.str() << std::endl;
    job.setJobDescription(os_str.str());
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
      os_str.clear();
      os_str << "vishnu_job_submit:" << errmsg << std::endl;

    }
    else
    {
      os_str << "vishnu_job_submit: Error (" << pbs_errno << "-";
      os_str << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    std::cout << os_str.str() << std::endl;
    job.setJobDescription(os_str.str());

    return -1;
  }

  pbs_disconnect(connect);

  job.setJobId(std::string(jobId));

  return 0;
}

