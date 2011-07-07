/**
 * \file get_system_info.cpp
 * This file defines the VISHNU command to get the system information
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
#include "displayer.hpp"

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct GetSysInfoFunc {

  IMS_Data::SysInfoOp msysInfoOp;

  GetSysInfoFunc(const IMS_Data::SysInfoOp& sysInfoOp):
   msysInfoOp(sysInfoOp)
  {};

  int operator()(std::string sessionKey) {
    IMS_Data::ListSysInfo listSysInfo;    
    int res = getSystemInfo(sessionKey, listSysInfo, msysInfoOp);
    displayListSysInfo(&listSysInfo);
    return res;
  }
};


boost::shared_ptr<Options>
makeGetystemInfoOpt(string pgName,
                  boost::function1<void, string>& fMachineId,
                  string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

   opt->add("machineId,m",
            "represents the id of the machine",
            CONFIG,
            fMachineId);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;

   /********** EMF data ************/
  IMS_Data::SysInfoOp sysInfoOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fMachineId (boost::bind(&IMS_Data::SysInfoOp::setMachineId,boost::ref(sysInfoOp),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeGetystemInfoOpt(argv[0], fMachineId, dietConfig);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  GetSysInfoFunc metricGetSysInfoFunc(sysInfoOp);
  return GenericCli().run(metricGetSysInfoFunc, dietConfig, argc, argv);
}

