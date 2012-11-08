/**
 * \file set_system_info.cpp
 * This file defines the VISHNU command to restart a vishnu process
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

struct RestartFunc {

  std::string mmachineId;
  IMS_Data::SupervisorOp mrestartOp;

  RestartFunc(std::string& machineId, const IMS_Data::SupervisorOp& restartOp):
   mmachineId(machineId), mrestartOp(restartOp)
  {};

  int operator()(std::string sessionKey) {
    return restart(sessionKey, mmachineId, mrestartOp);
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  string vishnu;
  string type;

   /********** EMF data ************/
  IMS_Data::SupervisorOp restartOp;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add( "machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);

  opt->setPosition("machineId",1);

  // All cli options
  opt->add("type,n",
           "The name of the element",
           CONFIG,
	   type,1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId sedType");

  restartOp.setName(type);
  //call of the api function
  RestartFunc restartFunc(machineId, restartOp);
  return GenericCli().run(restartFunc, dietConfig, argc, argv);

}

