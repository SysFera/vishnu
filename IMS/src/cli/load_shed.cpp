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

struct LoadShedFunc {

 IMS_Data::LoadShedType mloadShedType;
 std::string mmachineId;
 IMS_Data::SupervisorOp mop;

  LoadShedFunc(const IMS_Data::LoadShedType& loadShedType, const std::string& machineId, const IMS_Data::SupervisorOp& op):
    mloadShedType(loadShedType), mmachineId(machineId), mop(op)
  {};

  int operator()(std::string sessionKey) {
    return loadShed(sessionKey, mmachineId, mloadShedType, mop);
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;

   /********** EMF data ************/
  IMS_Data::LoadShedType loadShedType;
  IMS_Data::SupervisorOp op;

  /**************** Describe options *************/
  boost::function1<void,string> fsc(boost::bind(&IMS_Data::SupervisorOp::setURI,boost::ref(op),_1));
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

  opt->add( "loadShedType,l",
            " represents the type of the load shedding. The different values are:\n"
            "1 for HARD\n"
            "2 for SOFT",
            HIDDEN,
            loadShedType,1);

  opt->setPosition("loadShedType",1);

  opt->add("script,s",
	   "The supervisor script",
	   CONFIG,
	   fsc);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId loadShedType \n\nloadShedType values: 1 for HARD and 2 for SOFT");

  //call of the api function
  LoadShedFunc loadShedFunc(loadShedType, machineId, op);
  return GenericCli().run(loadShedFunc, dietConfig, argc, argv);
}

