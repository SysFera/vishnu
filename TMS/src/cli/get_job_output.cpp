/**
 * \file get_job_output.cpp
 * This file defines the VISHNU get job output command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "utils.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To display the output of a job
 * \fn void displayJobProgress(TMS_Data::JobResult_ptr j)
 * \param out: the outputpath of the job
 * \param err: the errorpath of the job
 */
void 
displayJobOutput(TMS_Data::JobResult_ptr j){
  // TODO
}




int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string jobId;

  /********** EMF data ************/

  /******** Callback functions ******************/
     
  /*********** Out parameters *********************/
  TMS_Data::JobResult_ptr out;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt (new Options(argv[0]));

  opt->setPosition("machineId",1);
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);

  opt->setPosition("jobId",1);
  opt->add("jobId,j",
	   "represents the id of the job",
	   HIDDEN,
	   jobId,1);
 
  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper(), "vishnu_get_job_out_put machineId jobId");

  if (ret != CLI_SUCCESS){
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options]  ");  
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

    // DIET call : get job output
    if(false==sessionKey.empty()){
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      getJobOutput(sessionKey, machineId, jobId, out);
    }

    displayJobOutput(out);

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
