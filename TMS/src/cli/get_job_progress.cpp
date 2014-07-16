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

  TMS_Data::ProgressOptions mprogOp;

  JobProgressFunc(const TMS_Data::ProgressOptions& progOp):
    mprogOp(progOp)
  {

  }

  int operator()(std::string sessionKey) {
    TMS_Data::ListProgression prog;
    int res = getJobProgress(sessionKey, prog, mprogOp);
    if (mprogOp.getJobId().empty() && mprogOp.getUser().empty()) {
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
 * \param setJobIdFct: Function to set the job id in the option object
 * \param setUser: Function to set the job owner in the option object
 * \param setMachineIdFct: Function to set the target machine in the option object
 * \param configFile: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeGetJobProgOp(std::string pgName,
                 boost::function1<void, string>& setJobIdFct,
                 boost::function1<void, string>& setUser,
                 boost::function1<void, string>& setMachineIdFct,
                 std::string& sessionKey,
                 std::string& configFile) {
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  opt->add("sessionkey,k",
      "VISHNU session key to connect",
      ENV,
      sessionKey);

  // All cli options
  opt->add("jobId,i",
           "The id of the job",
           CONFIG,
           setJobIdFct);
  opt->add("user,u",
           "The owner of the job",
           CONFIG,
           setUser);
  opt->add("machine,m",
           "The ID of the target machine",
           CONFIG,
           setMachineIdFct);

  return opt;
}


int main (int argc, char* argv[]){

  std::string configFile;
  std::string sessionKey;

  // Declare the obtions object, the related callbacks
  TMS_Data::ProgressOptions progOp;
  boost::function1<void,string> setJobIdFct(boost::bind(&TMS_Data::ProgressOptions::setJobId,boost::ref(progOp),_1));
  boost::function1<void,string> setUserFct(boost::bind(&TMS_Data::ProgressOptions::setUser,boost::ref(progOp),_1));
  boost::function1<void,string> setMachineIdFct(boost::bind(&TMS_Data::ProgressOptions::setMachineId,boost::ref(progOp),_1));

  boost::shared_ptr<Options> opt = makeGetJobProgOp(argv[0], setJobIdFct, setUserFct, setMachineIdFct, sessionKey, configFile);

  //To process list optionss
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  JobProgressFunc jobProgressFunc(progOp);
  return GenericCli().run(jobProgressFunc, configFile, argc, argv, sessionKey);
}
