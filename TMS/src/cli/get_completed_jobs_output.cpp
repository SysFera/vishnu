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

  TMS_Data::JobOutputOptions moptions;

  JobResultsFunc(const TMS_Data::JobOutputOptions& options)
    : moptions(options)
  {
  }

  int operator()(const std::string& sessionKey) {
    TMS_Data::ListJobResults jobResults;
    int res = getCompletedJobsOutput(sessionKey, jobResults, moptions);
    displayAllJobOutput(jobResults);
    return res;
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string configFile;
  string machineId;
  std::string outputDir;
  int forceDownloadDays = -1;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt (new Options(argv[0]));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  // Other options
  opt->add("machineId,m",
           "The target machine",
           CONFIG,
           machineId);
  opt->add("outDir,o",
           "The outputh dir of the jobs results",
           CONFIG,
           outputDir);
  opt->add("days,d",
           "Considers jobs submitted in the last <days> days",
           CONFIG,
           forceDownloadDays);

  // Process the options
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  TMS_Data::JobOutputOptions options;
  options.setMachineId(machineId);
  options.setOutputDir(outputDir);
  options.setDays(forceDownloadDays);

  // Process the command
  JobResultsFunc jobResultsFunc(options);
  return GenericCli().run(jobResultsFunc, configFile, argc, argv);
}
