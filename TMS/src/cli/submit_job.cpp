/**
 * \file submit_job.cpp
 * This file defines the VISHNU submit job command 
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param fname: The name of the job
 * \param fqueue : The queue to set the job
 * \param fwallTime : The wall time for the job
 * \param fmemory : The memory needed by the job
 * \param fnbCpu : The number of cpu needed
 * \param fnbNodeAndCpu : The number of node and processor per node
 * \param foutput : The output path
 * \param ferr : The error path
 * \param dietConfig: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeSubJobOp(string pgName, 
	     boost::function1<void, string>& fname,
	     boost::function1<void, string>& fqueue,
	     boost::function1<void, int>& fwallTime, 
	     boost::function1<void, int>& fmemory, 
	     boost::function1<void, int>& fnbCpu,
	     boost::function1<void, int>& fnbNodeAndCpu,
	     boost::function1<void, string>& foutput,
	     boost::function1<void, string>& ferr,
	     string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options
  opt->add("name,n",
	   "The name of the job",
	   CONFIG,
	   fname);
  opt->add("queue,q",
	   "The queue to submit the job",
	   CONFIG,
	   fqueue);
  opt->add("wallTime,t",
	   "The wall time for the job",
	   CONFIG,
	   fwallTime);
  opt->add("memory,m",
	   "The memory needed by the job",
	   CONFIG,
	   fmemory);
  opt->add("nbCpu,P",
	   "The number of cpu needed by the job",
	   CONFIG,
	   fnbCpu);
  opt->add("nbNodesAndCpuPerNode,N",
	   "The number of node and processor per node",
	   CONFIG,
	   fnbNodeAndCpu);
  opt->add("outPutPath,o",
	   "The outputh path of the job",
	   CONFIG,
	   foutput);
  opt->add("errorPath,e",
	   "The path for the job errors",
	   CONFIG,
	   ferr);

  return opt;
}


int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string scriptPath;

  /********** EMF data ************/
  TMS_Data::SubmitOptions subOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fname(boost::bind(&TMS_Data::SubmitOptions::setName,boost::ref(subOp),_1));
  boost::function1<void,string> fqueue(boost::bind(&TMS_Data::SubmitOptions::setQueue,boost::ref(subOp),_1));
  boost::function1<void,int> fwallTime(boost::bind(&TMS_Data::SubmitOptions::setWallTime,boost::ref(subOp),_1));
  boost::function1<void,int> fmemory(boost::bind(&TMS_Data::SubmitOptions::setMemory,boost::ref(subOp),_1));
  boost::function1<void,int> fnbCpu(boost::bind(&TMS_Data::SubmitOptions::setNbCpu,boost::ref(subOp),_1));
  boost::function1<void,int> fnbNodeAndCpu(boost::bind(&TMS_Data::SubmitOptions::setNbNodesAndCpuPerNode,boost::ref(subOp),_1));
  boost::function1<void,string> foutput(boost::bind(&TMS_Data::SubmitOptions::setOutputPath,boost::ref(subOp),_1));
  boost::function1<void,string> ferr(boost::bind(&TMS_Data::SubmitOptions::setErrorPath,boost::ref(subOp),_1));

     
  /*********** Out parameters *********************/
  Job job;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeSubJobOp(argv[0],fname,fqueue,fwallTime, 
					      fmemory, fnbCpu, fnbNodeAndCpu,
					      foutput, ferr, dietConfig);

  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  opt->add("scriptPath,p",
	   "represents the script of submission",
	   HIDDEN,
	   scriptPath,1);
  opt->setPosition("scriptPath",1);

  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] machineId script");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] machineId script");
    return 0;
  }

  // Process command
  try {

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : submit
    if(false==sessionKey.empty()){
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      submitJob(sessionKey, machineId, scriptPath, job, subOp);
    }
    displaySubmit(job);
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(argv[0], msg,EXECERROR);
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
