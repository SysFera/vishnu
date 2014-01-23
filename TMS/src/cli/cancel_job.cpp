/**
 * \file cancel_job.cpp
 * This file defines the VISHNU command to cancel a job
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
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

struct CancelJobFunc {

  TMS_Data::CancelOptions moptions;

  CancelJobFunc(const TMS_Data::CancelOptions& options):
    moptions(options)
  {

  }

  int operator()(std::string sessionKey) {
    return vishnu::cancelJob(sessionKey, moptions);
  }
};


/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param setJobIdFct: Function to set the job id in the option object
 * \param setMachineIdFct: Function to set the target machine in the option object
 * \param setUserFct: Function to set the target machine in the option object
 * \param configFile: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>
makeCancelJobOption(std::string pgName,
                    boost::function1<void, std::string>& setJobIdFct,
                    boost::function1<void, std::string>& setMachineIdFct,
                    boost::function1<void, std::string>& setUserFct,
                    std::string& configFile) {
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  // All cli options
  opt->add("job,j",
           "The id of the job",
           CONFIG,
           setJobIdFct);
  opt->add("machine,m",
           "The ID of the target machine",
           CONFIG,
           setMachineIdFct);
  opt->add("user,u",
           "Delete only jobs submitted by the given user",
           CONFIG,
           setUserFct);

  return opt;
}

int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  std::string configFile;

  TMS_Data::CancelOptions options;
  boost::function1<void,std::string> setJobIdFct(boost::bind(&TMS_Data::CancelOptions::setJobId,boost::ref(options),_1));
  boost::function1<void,std::string> setMachineIdFct(boost::bind(&TMS_Data::CancelOptions::setMachineId,boost::ref(options),_1));
  boost::function1<void,std::string> setUserFct(boost::bind(&TMS_Data::CancelOptions::setUser,boost::ref(options),_1));

  // get options and process the request
  boost::shared_ptr<Options> opt = makeCancelJobOption(argv[0], setJobIdFct, setMachineIdFct, setUserFct, configFile);
  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  CancelJobFunc cancelJobFunc(options);
  return GenericCli().run(cancelJobFunc, configFile, argc, argv);

}
