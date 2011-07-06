/**
 * \file cancel_job.cpp
 * This file defines the VISHNU command to cancel a job
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct CancelJobFunc {

  std::string mmachineId;
  std::string mjobId;

  CancelJobFunc(const std::string& machineId, const std::string& jobId):
    mmachineId(machineId), mjobId(jobId)
  {};

  int operator()(std::string sessionKey) {
    return cancelJob(sessionKey, mmachineId, mjobId);
  }
};


int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  string jobId;

  /********** EMF data ************/

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt (new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli obligatory parameters
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  opt->add("jobId,j",
	   "represents the id of the job",
	   HIDDEN,
	   jobId,1);
  opt->setPosition("jobId",1);
 
  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId jobId");

  //call of the api function
  CancelJobFunc cancelJobFunc(machineId, jobId);
  return GenericCli().run(cancelJobFunc, dietConfig, argc, argv); 

}
