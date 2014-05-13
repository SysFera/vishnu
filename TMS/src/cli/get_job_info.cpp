/**
 * \file get_job_info.cpp
 * This file defines the VISHNU get job info command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;


class InfoJobFunc {
public:
  InfoJobFunc(const std::string& jobId, const std::string& machineId)
    : mjobId(jobId), mmachineId(machineId)
  {
  }

  int operator()(const std::string& sessionKey) {
    TMS_Data::Job job;
    int res = vishnu::getJobInfo(sessionKey, mjobId, mmachineId, job);
    displayJob(job);
    return res;
  }

private:
  std::string mjobId;
  std::string mmachineId;
};


int
main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  std::string configFile;
  std::string jobId;
  std::string machineId;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  opt->add("jobId,j",
           "The id of the job",
           HIDDEN,
           jobId,1);
  opt->setPosition("jobId",1);

  // All cli options
  opt->add("machine,m",
           "The id of the target machine. If not set, the request will be routed to the dispatcher",
           CONFIG,
           machineId);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  InfoJobFunc infoJobFunc(jobId, machineId);
  return GenericCli().run(infoJobFunc, configFile, argc, argv);
}
