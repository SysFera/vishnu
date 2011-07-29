/**
 * \file stop.cpp
 * This file defines the VISHNU command to stop a vishnu process
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct StopFunc {

  IMS_Data::Process mprocess;

  StopFunc(IMS_Data::Process process):
   mprocess(process)
  {};

  int operator()(std::string sessionKey) {
    return stop(sessionKey, mprocess);
  }
};


boost::shared_ptr<Options>
makeStopOpt(string pgName,
            boost::function1<void, string>& fDietId,
            string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

    // All cli options
   opt->add("dietId,d",
            "The diet id of the process",
            CONFIG,
            fDietId);

  return opt;
}


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  string processName;

   /********** EMF data ************/
  IMS_Data::Process process;

  /******** Callback functions ******************/
  boost::function1<void, string> fDietId (boost::bind(&IMS_Data::Process::setDietId,boost::ref(process),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeStopOpt(argv[0], fDietId, dietConfig);


  opt->add( "processName,p",
            "represents the name of the process",
            HIDDEN,
            processName,1);

  opt->setPosition("processName",1);

  opt->add( "machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);

  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "processName machineId");

  process.setMachineId(machineId);
  process.setProcessName(processName);
  //call of the api function
  StopFunc stopFunc(process);
  return GenericCli().run(stopFunc, dietConfig, argc, argv);

}

