/**
 * \file list_queues.cpp
 * This file defines the VISHNU list queues command 
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
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

struct ListQueuesFunc {

  std::string mmachineId;
  std::string mqueueName;

  ListQueuesFunc(const std::string& machineId, const std::string& queueName):
    mmachineId(machineId), mqueueName(queueName)
  {};

  int operator()(std::string sessionKey) {
    TMS_Data::ListQueues queue;
    int res = listQueues(sessionKey, mmachineId, queue, mqueueName);
    if (mqueueName.size()!=0){
      displayQueues(queue);
    } else {
      std::cout << queue << std::endl;
    }
  
      return res;
  }
};

int 
main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string machineId;
  string queueName;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));


  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("queueName,q",
          "An option for listing all information of the given\n"
          "queue name",
          CONFIG,
          queueName); 

  // All cli obligatory parameters
  opt->setPosition("machineId",1);
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  ListQueuesFunc listQueueFunc(machineId, queueName);
  return GenericCli().run(listQueueFunc, dietConfig, argc, argv);
}
