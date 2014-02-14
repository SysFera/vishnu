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
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct JobOutputJobFunc {

  TMS_Data::JobOutputOptions moptions;
  std::string mjobId;

  JobOutputJobFunc(const std::string& jobId, const TMS_Data::JobOutputOptions& options)
    : mjobId(jobId), moptions(options)
  {
  }

  int operator()(std::string sessionKey) {
    TMS_Data::JobResult jobResult;
    int res = getJobOutput(sessionKey, mjobId, jobResult, moptions);
    displayJobOutput(&jobResult);
    return res;
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string configFile;
  string jobId;
  string machineId;
  std::string outputDir;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);
  opt->add("jobId,j",
           "The id of the job",
           HIDDEN,
           jobId,1);
  opt->setPosition("jobId",1);

  opt->add("outDir,o",
           "The output directory",
           CONFIG,
           outputDir);
  opt->add("machineId,m",
           "The target machine",
           CONFIG,
           machineId);

  // process options
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  TMS_Data::JobOutputOptions options;
  options.setMachineId(machineId);
  options.setOutputDir(outputDir);

  // process the request
  JobOutputJobFunc jobOutputFunc(jobId, options);
  return GenericCli().run(jobOutputFunc, configFile, argc, argv);

}
