/**
 * \file get_job_progress.cpp
 * This file defines the VISHNU get job progress command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "displayer.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct JobProgressFunc {

  std::string mmachineId;
  TMS_Data::ProgressOptions mprogOp;

  JobProgressFunc(const std::string& machineId, const TMS_Data::ProgressOptions& progOp):
    mmachineId(machineId), mprogOp(progOp)
  {};

  int operator()(std::string sessionKey) {
    TMS_Data::ListProgression prog; 
    int res = getJobProgress(sessionKey, mmachineId, prog, mprogOp);
    if(mprogOp.getJobId().size()==0 && mprogOp.getJobOwner().size()==0) {
      std::cout << prog << std::endl;
    } else {
      displayJobProgress(prog);
    }
 
    return res;
  }
};


/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param fjid: The id of the job
 * \param fown: The owner of the job
 * \param dietConfig: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeGetJobProgOp(string pgName, 
		 boost::function1<void, string>& fjid,
		 boost::function1<void, string>& fown,
		 string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options
  opt->add("jobid,i",
	   "The id of the job",
	   CONFIG,
	   fjid);
  opt->add("jobOwner,u",
	   "The owner of the job",
	   CONFIG,
	   fown);

  return opt;
}


int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;

  /********** EMF data ************/
  TMS_Data::ProgressOptions progOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fjid(boost::bind(&TMS_Data::ProgressOptions::setJobId,boost::ref(progOp),_1));
  boost::function1<void,string> fown(boost::bind(&TMS_Data::ProgressOptions::setJobOwner,boost::ref(progOp),_1));

     
  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeGetJobProgOp(argv[0], fjid, fown, dietConfig);

  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId");

  //call of the api function
  JobProgressFunc jobProgressFunc(machineId, progOp);
  return GenericCli().run(jobProgressFunc, dietConfig, argc, argv);
}
