#include <vector>
#include <sstream>

extern "C" {
#include "pbs_ifl.h" //Torque includes
#include "pbs_error.h"
#include "cmds.h"
#include "pbs_sub.h"
#include "net_connect.h"
}

#include "TorqueServer.hpp"
#include "TMSVishnuException.hpp"

const int MAX_STRING_SIZE = 255;

TorqueServer::TorqueServer():BatchServer() {
}

int TorqueServer::submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job, char** envp) {

  char destination[PBS_MAXDEST];
  char script_tmp[MAXPATHLEN + 1] = "";
  char serverOut[PBS_MAXSERVERNAME + PBS_MAXPORTNUM + 2];
  struct attrl *attrib= NULL;
  char errMsg[1024];
  int argc = 0;
  
  std::ostringstream os_str;

  std::vector<std::string> cmds_options;
  //processes the options
  process_options(options, cmds_options);
  argc = cmds_options.size()+2;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  argv[1] = const_cast<char*>(scriptPath);
  for(int i=0; i < cmds_options.size(); i++) {
   argv[i+2] = const_cast<char*>(cmds_options[i].c_str());
  }

  destination[0] = '\0';
  serverOut[0] = '\0';
  //parses the scripthPath and sets the options values
  pbs_prepare_script(argc, argv, envp, script_tmp, destination, serverOut, &attrib);
  
  errMsg[0] = '\0';
  get_pbs_error_msg(errMsg);
  if(errMsg[0]!='\0') {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "TORQUE ERROR: "+std::string(errMsg));
  }

  int connect = cnt2server(serverOut);
  std::cerr << "******* connect = " << connect << std::endl;
  if (connect <= 0) {
    std::ostringstream connect_error;
    connect_error << "TORQUE ERROR: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    if (getenv("PBSDEBUG") != NULL)
    {
      connect_error << "TORQUE ERROR: pbs_server daemon may not be running on host";
      connect_error << "or hostname in file '$TORQUEHOME/server_name' may be incorrect" << std::endl;
    }
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());
  }
  
  pbs_errno = 0;
  char*jobId = pbs_submit(connect, (struct attropl *)attrib,
                          script_tmp, destination, NULL);

  if (jobId == NULL) {
    std::cerr << "******* jobId is NULL ...." << std::endl;
    std::ostringstream submit_error;     
    char* errmsg = pbs_geterrmsg(connect);

    if (errmsg != NULL)
    {
      submit_error << "pbs_submit: " << errmsg << std::endl;

    }
    else
    {
      submit_error << "pbs_submit: Error (" << pbs_errno << "-";
      submit_error << pbs_strerror(pbs_errno) << std::endl;
    }
    pbs_disconnect(connect);

    unlink(script_tmp);

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, submit_error.str());
  }

  pbs_disconnect(connect);
  unlink(script_tmp);

  job.setJobId(std::string(jobId));

  return 0;
}

void TorqueServer::process_options(const TMS_Data::SubmitOptions& options, std::vector<std::string>&cmds_options) {

  if(options.getName().size()!=0){
    cmds_options.push_back("-N");
    cmds_options.push_back(options.getName());
  }
  if(options.getQueue().size()!=0) {
    cmds_options.push_back("-q");
    cmds_options.push_back(options.getQueue());
  }
  if(options.getOutputPath().size()!=0) {
    cmds_options.push_back("-o");
    cmds_options.push_back(options.getOutputPath());
  }
  if(options.getErrorPath().size()!=0) {
    cmds_options.push_back("-e");
    cmds_options.push_back(options.getErrorPath());
  }
  if(options.getWallTime()!=-1) {
    cmds_options.push_back("-l"); //TODO:prendre bien en compte ce traitement dans pbs_prepare_script
    std::ostringstream os_str;
    os_str << options.getWallTime(); //TODO:A convertir en string correctement
    cmds_options.push_back("walltime="+os_str.str());
  }
  if(options.getNbCpu()!=-1) {
    cmds_options.push_back("-l");
    std::ostringstream os_str;
    //os_str.clear();
    os_str << options.getNbCpu();
    cmds_options.push_back("ncpus="+os_str.str());
  }
  if(options.getMemory()!=-1) {
    cmds_options.push_back("-l");
    //os_str.clear();
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmds_options.push_back("mem="+os_str.str());
  }
  if(options.getNbNodesAndCpuPerNode()!=-1) {//TODO: le type est string
    //cmds_options.push_back("-l");
    //cmds_options.push_back("nodes=1:ppn=2");
  }

}

int TorqueServer::cancel(const char* jobId) {
  char remoteServer[MAX_STRING_SIZE];
  return pbs_cancel(jobId, remoteServer);
}

int TorqueServer::pbs_cancel(const char* jobId, char remoteServer[], bool isLocal) {

  char tmsJobId[MAX_STRING_SIZE];  
  char tmsJobIdOut[MAX_STRING_SIZE];
  char serverOut[MAX_STRING_SIZE];
  int connect;

  if(isLocal) {
    strcpy(tmsJobId, jobId);
    if (get_server(tmsJobId, tmsJobIdOut, serverOut))
    {
      std::ostringstream jobIdError;
      jobIdError << "pbs_deljob: illegally formed job identifier: " << tmsJobId << std::endl;
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,  jobIdError.str());
    }
  }

  std::cout << "tmsJobIdOut " << tmsJobIdOut << std::endl;
  std::cout << "serverOut=" << serverOut << std::endl;
  
  if(isLocal) {
     connect = cnt2server(serverOut);
  } else {
     connect = cnt2server(remoteServer);
  }

  if (connect <= 0)
  {
    std::ostringstream connect_error;
    connect_error << "TORQUE ERROR: cannot connect to server ";
    connect_error <<  pbs_server << " (errno=" << pbs_errno << ") " << pbs_strerror(pbs_errno) << std::endl;

    //rt_job_err("qdel", connect, tmsJobIdOut);
    
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, connect_error.str());

  }

  pbs_errno = 0;
  int stat = pbs_deljob(connect, tmsJobIdOut, NULL);

  std::ostringstream pbs_del_error;
  if (stat && (pbs_errno != PBSE_UNKJOBID)) {
       pbs_del_error << "vishnu_cancel: " << connect << tmsJobIdOut << std::endl;
       TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, pbs_del_error.str());
       pbs_disconnect(connect);
  } else if (stat && (pbs_errno == PBSE_UNKJOBID) && isLocal ) {
    if (locate_job(tmsJobIdOut, serverOut, remoteServer)) { 
      pbs_disconnect(connect);
      pbs_cancel(tmsJobId,  remoteServer, false); 
      return 0;
    }

    pbs_del_error << "Unknown JobId " << tmsJobIdOut << std::endl;
    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, pbs_del_error.str());

  } else if(pbs_errno == PBSE_UNKJOBID) {
    pbs_del_error << "Unknown JobId " << tmsJobIdOut << std::endl;
    pbs_disconnect(connect);
    throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID, pbs_del_error.str());
  }

  pbs_disconnect(connect);
  return 0;
}

TorqueServer::~TorqueServer() { 
}
