/**
 * \file SGEServer.hpp
 * \brief This file contains the VISHNU SGEServer class.
 * \author Amine Bsila (amine.bsila@sysfera.com)
 * \date Febrary 2012
 */

#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>

#include <boost/algorithm/string.hpp>
#include <boost/process/all.hpp> 
#include <boost/process/stream_id.hpp>
#include <boost/process/stream_type.hpp>
#include <boost/assign/list_of.hpp> 

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


extern "C" {
#include "drmaa.h"

}

#include "SGEServer.hpp"
#include "BatchServer.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"
#include "Env.hpp"
using namespace std;
using namespace vishnu;
namespace bp = boost::process;

/**
 * \brief Constructor
 */
SGEServer::SGEServer():BatchServer() {
  msymbolMap["\%j"] = "";
  msymbolMap["\%J"] = "";
  
}

/**
 * \brief Function to submit SGE job
 * \param scriptPath the path to the script containing the job characteristique
 * \param options the options to submit job
 * \param job The job data structure
 * \param envp The list of environment variables used by SGE submission function 
 * \return raises an exception on error
 */
int 
SGEServer::submit(const char* scriptPath, 
    const TMS_Data::SubmitOptions& options, 
    TMS_Data::Job& job, char** envp) {

  
  drmaa_job_template_t *jt = NULL;
  int ret;
  int retries = 0;
  int VISHNU_MAX_RETRIES = 5;
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  int drmaa_errno;
  char jobid[100];
  char jobOutputPath[256] ;
  char jobErrorPath[256];
  char jobName[256];

  string Walltime;
  
  
  
  drmaa_errno = drmaa_allocate_job_template(&jt, diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, std::string(diagnosis));

  }
    
  std::string scriptContent = vishnu::get_file_content(scriptPath);
     
  std::istringstream iss(scriptContent);
  std::string line;
  std::string scriptoption;
  std::vector<std::string> cmdsOptions;
  std::string value;
  
  while(!iss.eof()) {
    getline(iss, line);
    size_t pos = line.find('#');
    if(pos==string::npos) {
      
      continue;
    }
    line = line.erase(0, pos);
    if(boost::algorithm::starts_with(line, "#$")){
      
      line = line.substr(std::string("#$").size());
      pos = line.find("-N");
      if(pos!=std::string::npos){
	
        value = line.substr(pos+3);
        drmaa_errno = drmaa_set_attribute(jt,DRMAA_JOB_NAME,value.c_str(),diagnosis, sizeof(diagnosis)-1);
        if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
	  
          throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
          
        }


      } else {
        
        pos = line.find("-o");
        if(pos!=std::string::npos){
	  
          if(boost::algorithm::contains(line, ":")){
	    
              value = line.substr(pos+3);
            } else{
	      
              value = ":"+line.substr(pos+3);              
            }
          drmaa_errno = drmaa_set_attribute(jt,DRMAA_OUTPUT_PATH,value.c_str(),diagnosis, sizeof(diagnosis)-1);
          if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){

            throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
            
          }
        } else {
          
          pos = line.find("-e");
          if(pos!=std::string::npos){
            
            if(boost::algorithm::contains(line, ":")){
	      
              value = line.substr(pos+3);
	      
            } else{
	      
              value = ":"+line.substr(pos+3); 
	      
            }
            drmaa_errno = drmaa_set_attribute(jt,DRMAA_ERROR_PATH,value.c_str(),diagnosis, sizeof(diagnosis)-1);
            if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){

              throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
              
            }            
          } else{
	    
            scriptoption.append(line);
          
          }
          
        }    
      }  
    }
  }
  
  drmaa_errno = drmaa_set_attribute(jt, DRMAA_REMOTE_COMMAND, scriptPath , diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  } 
  processOptions(scriptPath,options,cmdsOptions,jt);
  
  for(int i=0; i < cmdsOptions.size(); i++) {
    
    scriptoption += const_cast<char*>(cmdsOptions[i].c_str());
    cout << "command line is " << cmdsOptions[i] << endl;
    if (boost::algorithm::starts_with(cmdsOptions[i], "s_rt")){
      
      Walltime = cmdsOptions[i].substr(5);
      job.setWallClockLimit(vishnu::convertStringToWallTime(Walltime));  
    }  
    
    
  }
  
  
  drmaa_errno = drmaa_set_attribute(jt, DRMAA_NATIVE_SPECIFICATION, scriptoption.c_str(),diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  }
  
  
  
  //To submit the job
  while ((drmaa_errno=drmaa_run_job(jobid, sizeof(jobid)-1, jt, diagnosis,
               sizeof(diagnosis)-1)) == DRMAA_ERRNO_DRM_COMMUNICATION_FAILURE) {
    
         cout<<"[drmaa_run_job DRM Comunication Failure] " << diagnosis << endl;
         sleep(1);
  
  }
      
  
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){

    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  }   
  
  int size=256;
  char jobid_out[DRMAA_JOBNAME_BUFFER];
  int status = 0;
  drmaa_attr_values_t *rusage = NULL;
  int errnum = 0;
  char usage[DRMAA_ERROR_STRING_BUFFER];
  
  drmaa_errno = drmaa_get_attribute(jt,DRMAA_ERROR_PATH,jobErrorPath, size,diagnosis, sizeof(diagnosis)-1);

  if (drmaa_errno==DRMAA_ERRNO_SUCCESS){
    
    std::string jobErrorPathStr = jobErrorPath;
    Env(SGE).replaceAllOccurences(jobErrorPathStr,"$JOB_ID",jobid);
    if(boost::algorithm::contains(jobErrorPathStr, "$")){
      
      throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use another envirnment variable than $JOB_ID.\n");
      
    }
    job.setErrorPath(jobErrorPathStr);
    
  }
  drmaa_errno = drmaa_get_attribute(jt,DRMAA_OUTPUT_PATH,jobOutputPath, size,diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno==DRMAA_ERRNO_SUCCESS){
    std::string jobOutputPathStr = jobOutputPath;
    Env(SGE).replaceAllOccurences(jobOutputPathStr,"$JOB_ID",jobid);
    if(boost::algorithm::contains(jobOutputPathStr, "$")){
      
      throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use another envirnment variable than $JOB_ID.\n"); 
      
    }
    job.setOutputPath(jobOutputPathStr);
    
  }
  drmaa_errno = drmaa_get_attribute(jt,DRMAA_JOB_NAME,jobName,size,diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno==DRMAA_ERRNO_SUCCESS){
    
    job.setJobName(jobName);
    
  }
     
  job.setStatus(getJobState(jobid));
  job.setJobId(jobid);
  
  drmaa_errno = drmaa_delete_job_template(jt, diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
    
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  }
  return 0;
}

int
SGEServer::cancel(const char* jobId) {
  
  
  
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  int drmaa_errno;
  drmaa_errno = drmaa_control(jobId, DRMAA_CONTROL_TERMINATE, diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  }
  
  return 0;
}

/**
 * \brief Function to get the status of the job
 * \param jobId the identifier of the job 
 * \return -1 if the job is unknown or server not  unavailable 
 */
int 
SGEServer::getJobState(const std::string& jobId) {
  
  int ret=0;
  int state=-1;
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  drmaa_job_ps(jobId.c_str(), &state, diagnosis, sizeof(diagnosis)-1);
  
  switch (state) {
    case DRMAA_PS_UNDETERMINED:
     ret = -1;
     break;
    case DRMAA_PS_QUEUED_ACTIVE:case DRMAA_PS_SYSTEM_ON_HOLD:case DRMAA_PS_USER_ON_HOLD:case DRMAA_PS_USER_SYSTEM_ON_HOLD:case DRMAA_PS_SYSTEM_SUSPENDED:case DRMAA_PS_USER_SUSPENDED:case DRMAA_PS_USER_SYSTEM_SUSPENDED:
     ret = 3;
     break;
   case DRMAA_PS_RUNNING:
     ret = 4;//RUNNING
     break;
   case DRMAA_PS_DONE:case DRMAA_PS_FAILED:
     ret = 5;
     break;
   default:
     ret = 5;
     break;
    
  } /* switch */

  return ret;
}

/**
 * \brief Function to get the start time of the job
 * \param jobId the identifier of the job 
 * \return 0 if the job is unknown or server not  unavailable
 */
time_t 
SGEServer::getJobStartTime(const std::string& jobId) {


  time_t startTime = 0;
  int status = 0;
  int state=-1;
  size_t pos;   
  drmaa_attr_values_t *rusage = NULL;
  char jobid_out[DRMAA_JOBNAME_BUFFER];
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  char usage[DRMAA_ERROR_STRING_BUFFER];
  int drmaa_errno; 
  drmaa_errno = drmaa_job_ps(jobId.c_str(), &state, diagnosis, sizeof(diagnosis)-1);
  
  if (state==DRMAA_PS_RUNNING){
    std::string exe = boost::process::find_executable_in_path("qstat"); 
    std::vector<std::string> args; 
    boost::process::context ctx; 
    ctx.streams[boost::process::stdout_id] = boost::process::behavior::pipe(); 
    boost::process::child c = boost::process::create_child(exe, args, ctx); 
    boost::process::pistream is(c.get_handle(boost::process::stdout_id));
    
    
    std::string line;
    std:: string time;
    
    while (std::getline(is, line)){  
      boost::algorithm::trim_left(line);
      if(boost::algorithm::starts_with(line, jobId.c_str())){
	pos = line.find(" r ");
	if(pos!=std::string::npos){
	  line = line.substr(pos+2);
	  boost::algorithm::trim_left(line);
	  time = line.substr(0,19);
	  struct tm tm_obj;
	  strptime(time.c_str(), "%m/%d/%Y %H:%M:%S", &tm_obj);
	  startTime = mktime(&tm_obj);
	} 
      }
      
    }
    
  }  
  

  return startTime;
}

/**
 * \brief Function to request the status of queues 
 * \param optQueueName (optional) the name of the queue to request 
 * \return The requested status in to ListQueues data structure 
 */
TMS_Data::ListQueues*
SGEServer::listQueues(const std::string& optqueueName) { 

  TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
  mlistQueues = ecoreFactory->createListQueues();
  std::string exe = boost::process::find_executable_in_path("qconf");
  std::vector<std::string> args;
  std::vector<std::string> argss;
  std::vector<std::string> queueNames;
  boost::process::context ctx;
  ctx.streams[boost::process::stdout_id] = boost::process::behavior::pipe();
  args.push_back("-sq");
  if(optqueueName.size()!=0) {
    queueNames.push_back(optqueueName.c_str());
  } else {
    argss.push_back("-sql");
    boost::process::child c = boost::process::create_child(exe, argss, ctx);
    boost::process::pistream is(c.get_handle(boost::process::stdout_id));
    std::string line;
   
    while (std::getline(is, line)){
      queueNames.push_back(line.c_str());
    } 
    
  }
  vector<std::string>::iterator it;
  
  
  for ( it=queueNames.begin() ; it < queueNames.end(); it++ ){

    args.push_back((*it).c_str());
    boost::process::child c1 = boost::process::create_child(exe, args, ctx);
    boost::process::pistream iss(c1.get_handle(boost::process::stdout_id));
    std::string line;
    args.pop_back();
    if (iss){

      TMS_Data::Queue_ptr queue = ecoreFactory->createQueue();
    
      while (std::getline(iss, line)){

        if(boost::algorithm::starts_with(line, "qname")){
          line = line.substr(5);
          boost::algorithm::trim_left(line);
          queue->setName(line);

        }
        if(boost::algorithm::starts_with(line, "priority")){
          line = line.substr(8);
          boost::algorithm::trim_left(line);
          int priority = atoi(line.c_str());
          queue->setPriority(priority);

        }
        /*if(boost::algorithm::starts_with(line, "qname")){
          
        }
        if(boost::algorithm::starts_with(line, "qname")){

        }*/


      }
      queue->setState(2);
      queue->setNbRunningJobs(1);
      queue->setNbJobsInQueue(0);
      queue->setDescription("");//UNDEFINED in SGE
      queue->setMaxProcCpu(1);
      queue->setMaxJobCpu(1);
      queue->setNode(1);
      
      mlistQueues->getQueues().push_back(queue);
    }
    
  }
  mlistQueues->setNbQueues(mlistQueues->getQueues().size());
    

  return mlistQueues;
}

void SGEServer::fillListOfJobs(TMS_Data::ListJobs*& listOfJobs,
                                  const std::vector<string>& ignoredIds) {
                                    
}

/**
 * \brief Destructor
 */
SGEServer::~SGEServer() {
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  int drmaa_errno;
  drmaa_errno = drmaa_exit(diagnosis, sizeof(diagnosis)-1);
  if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
    
  }
}




/**
 * \brief Function to treat the submission options
 * \param scriptPath The job script path
 * \param options the object which contains the SubmitOptions options values
 * \param cmdsOptions The list of the option value
 * \return raises an exception on error
 */
void 
SGEServer::processOptions(const char* scriptPath,
                    const TMS_Data::SubmitOptions& options, 
                    std::vector<std::string>&cmdsOptions,
                    drmaa_job_template_t *jobt) {

  /*if(!options.getNbNodesAndCpuPerNode().empty() && options.getNbCpu()!=-1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the NbCpu option and NbNodesAndCpuPerNode option together.\n");
  }
  
  if(options.isSelectQueueAutom() && !options.getQueue().empty() ) {
    throw UserException(ERRCODE_INVALID_PARAM, "Conflict: You can't use the SelectQueueAutom (-Q) and getQueue (-q) options together.\n");
  }*/
  char diagnosis[DRMAA_ERROR_STRING_BUFFER];
  int drmaa_errno;
  if(options.getName().size()!=0){
    //cmdsOptions.push_back(" -N ");
    std::ostringstream os_str;
    os_str << options.getName();
    drmaa_errno = drmaa_set_attribute(jobt,DRMAA_JOB_NAME,os_str.str().c_str(),diagnosis,sizeof(diagnosis)-1);
    if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
      
    }
  }
  if(options.getQueue().size()!=0) {
    cmdsOptions.push_back(" -q ");
    cmdsOptions.push_back(options.getQueue());
  }
  if(options.getOutputPath().size()!=0) {
    std::ostringstream os_str;
    if(boost::algorithm::contains(options.getOutputPath(), ":")){
       os_str << options.getOutputPath();
    } else{
        os_str << ":" << options.getOutputPath();              
    }
    
    drmaa_errno = drmaa_set_attribute(jobt,DRMAA_OUTPUT_PATH,os_str.str().c_str(),diagnosis,sizeof(diagnosis)-1);
    if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
      
    }
  }
  if(options.getErrorPath().size()!=0) {
    std::ostringstream os_str;
    
    if(boost::algorithm::contains(options.getErrorPath(), ":")){
       os_str << options.getErrorPath();
    } else{
        os_str << ":"<<options.getErrorPath();              
    }
    
    drmaa_errno = drmaa_set_attribute(jobt,DRMAA_ERROR_PATH,os_str.str().c_str(),diagnosis,sizeof(diagnosis)-1);
    if (drmaa_errno!=DRMAA_ERRNO_SUCCESS){
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SGE ERROR: "+std::string(diagnosis));
      
    }
  }
  if(options.getWallTime()!=-1) {
    cmdsOptions.push_back(" -l "); 
    cmdsOptions.push_back("s_rt="+vishnu::convertWallTimeToString(options.getWallTime()));
    
  }
  /*if(options.getNbCpu()!=-1) {l

    cmdsOptions.push_back("-l");

    string format = getFormatedCpuPerNode(options.getNbCpu(), scriptPath);

    if(!format.empty()) {
      cmdsOptions.push_back(format);
    } else {
      cmdsOptions.push_back("nodes=1:ppn="+vishnu::convertToString(options.getNbCpu()));
    }

  }*/
  if(options.getMemory()!=-1) {
    cmdsOptions.push_back(" -l ");
    std::ostringstream os_str;
    os_str << options.getMemory();
    cmdsOptions.push_back(" s_vmem="+os_str.str()+"M");
  }
  
  if(options.getMailNotification()!="") {
    cmdsOptions.push_back(" -m ");  
    std::string notification = options.getMailNotification();
    if(notification.compare("BEGIN")==0) {
      cmdsOptions.push_back("b"); 
    } else if(notification.compare("END")==0) {
      cmdsOptions.push_back("e");
    } else if(notification.compare("ERROR")==0) {
      cmdsOptions.push_back("a");
    } else if(notification.compare("ALL")==0) {
      cmdsOptions.push_back("abe");
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, notification+" is an invalid notification type:"+" consult the vishnu user manuel.\n");
      
    }
  }

  if(options.getMailNotifyUser()!="") {
    cmdsOptions.push_back(" -M ");
    cmdsOptions.push_back(options.getMailNotifyUser());
  } 

  /*if(options.getGroup()!="") {
    cmdsOptions.push_back("-W");
    cmdsOptions.push_back("group_list="+options.getGroup());
  }*/

  if(options.getWorkingDir()!="") {
    cmdsOptions.push_back(" -wd ");
    cmdsOptions.push_back(options.getWorkingDir());
  }

  if(options.getCpuTime()!="") {
    cmdsOptions.push_back(" -l ");
    cmdsOptions.push_back("s_cpu="+options.getCpuTime());
  }
  
}

