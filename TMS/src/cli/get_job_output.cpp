/**
 * \file get_job_output.cpp
 * This file defines the VISHNU get job output command
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

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct JobOutputJobFunc {

  std::string mmachineId;
  std::string mjobId;
  std::string moutDir;

  JobOutputJobFunc(const std::string& machineId, const std::string& jobId, const std::string& outDir):
    mmachineId(machineId), mjobId(jobId), moutDir(outDir)
  {};

  int operator()(std::string sessionKey) {
    TMS_Data::JobResult jobResult;
    int res = getJobOutput(sessionKey, mmachineId, mjobId, jobResult, moutDir);
    displayJobOutput(&jobResult);
    return res;
  }
};


int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  string jobId;
  std::string outDir;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

   opt->add("outDir,o",
     "The outputh dir of the job output",
     CONFIG,
     outDir);
   
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
  JobOutputJobFunc jobOutputFunc(machineId, jobId, outDir);
  return GenericCli().run(jobOutputFunc, dietConfig, argc, argv);
  
}
