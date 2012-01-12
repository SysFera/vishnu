/**
 * \file get_job_output.cpp
 * This file defines the VISHNU get job output command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct JobResultsFunc {

  std::string mmachineId;
  std::string moutDir;

  JobResultsFunc(const std::string& machineId, const std::string& outDir):
    mmachineId(machineId), moutDir(outDir)
  {};

  int operator()(std::string sessionKey) {
    TMS_Data::ListJobResults jobResults;
    int res = getCompletedJobsOutput(sessionKey, mmachineId, jobResults, moutDir);
    displayAllJobOutput(jobResults); 
    return res;
  }
};


int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  std::string outDir;

   /**************** Describe options *************/
  boost::shared_ptr<Options> opt (new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("outDir,o",
     "The outputh dir of the jobs results",
     CONFIG,
     outDir);

  // All cli obligatory parameters
  opt->add("machineId,m",
     "represents the id of the machine",
     HIDDEN,
     machineId,1);
  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId");

  //call of the api function
  JobResultsFunc jobResultsFunc(machineId, outDir);
  return GenericCli().run(jobResultsFunc, dietConfig, argc, argv);
}
