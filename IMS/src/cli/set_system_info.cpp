/**
 * \file set_system_info.cpp
 * This file defines the VISHNU command to set the system information
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

struct SetSysInfoFunc {

  IMS_Data::SystemInfo msysInfo;

  SetSysInfoFunc(IMS_Data::SystemInfo sysInfo):
   msysInfo(sysInfo)
  {};

  int operator()(std::string sessionKey) {
    return setSystemInfo(sessionKey, msysInfo);
  }
};

boost::shared_ptr<Options>
makeSystemInfoOpt(string pgName,
                  boost::function1<void, long>& fMemory,
                  boost::function1<void, long>& fDiskSpace,
                  string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

   opt->add("memory,m",
            "Amount of disk space available on the machine (in Bytes)",
            CONFIG,
            fMemory);

  // All cli options
  opt->add("diskSpace,d",
           "Amount of RAM memory available on the machine (in Bytes)",
            CONFIG,
            fDiskSpace);

  return opt;
}


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;

   /********** EMF data ************/
  IMS_Data::SystemInfo sysInfo;

  /******** Callback functions ******************/
  boost::function1<void,long> fMemory (boost::bind(&IMS_Data::SystemInfo::setMemory,boost::ref(sysInfo),_1));
  boost::function1<void,long> fDiskSpace (boost::bind(&IMS_Data::SystemInfo::setDiskSpace,boost::ref(sysInfo),_1));
  /*********** Out parameters *********************/

  /**************** Describe options *************/


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeSystemInfoOpt(argv[0], fMemory, fDiskSpace, dietConfig);

  opt->add( "machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);

  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId");

  //call of the api function
  SetSysInfoFunc setSysInfoFunc(sysInfo);
  return GenericCli().run(setSysInfoFunc, dietConfig, argc, argv);

}

