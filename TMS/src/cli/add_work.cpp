/**
 * \file add_machine.cpp
 * This file defines the VISHNU add machine command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
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

boost::shared_ptr<Options> makeWorkOptions(std::string pgName, std::string & dietConfig,
                                           boost::function1<void, string> & fapp, boost::function1<void, string>& fsub,
                                           boost::function1<void, int>& fpri,  boost::function1<void, string>& fown, boost::function1<void, int>& fest,

                                           boost::function1<void, string>& fdesc, boost::function1<void, string>& fpro,
                                           boost::function1<void, string>& fmac,
                                           boost::function1<void, int>& fcpu, int type){

  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);


  Group_type group=CONFIG;


  if(type){// type =0 for "update function" and type=1 for "add function"

    group=HIDDEN;
  }


  opt->add("applicationId,a",
           "The id of the application",
           group,
           fapp,
           type);

  if (type){
    opt->setPosition("applicationId",1);
  }

  opt->add("subject,s",
           "The subject of the work",
           group,
           fsub,
           type);

  if (type){
    opt->setPosition("subject",1);
  }

  opt->add("priority,l",
           "The priority for the work",
           group,
           fpri,
           type);

  if (type){
    opt->setPosition("priority",1);
  }

  opt->add("owner,u",
           "The owner of the work",
           group,
           fown,
           type);

  if(type){

    opt->setPosition("owner",1);
  }

  opt->add("estimatedHour,e",
           "The estimation of the duration of the work",
           group,
           fest,
           type);

  if(type){
    opt->setPosition("estimatedHour",1);
  }


  opt->add("description,d",
           "The description of the work",
           group,
           fdesc,
           type);

  if(type){
    opt->setPosition("description",1);
  }

  opt->add("projectId,p",
           "The id of the project",
           group,
           fpro,
           type);

  if(type){
    opt->setPosition("projectId",1);
  }

  opt->add("machineId,m",
           "The machineId",
           group,
           fmac,
           type);

  if(type){
    opt->setPosition("machineId",1);
  }
  opt->add("nbcpu,n",
           "The number of cpu",
           group,
           fcpu,
           type);

  if(type){
    opt->setPosition("nbcpu",1);
  }

  return opt;



}



struct AddWorkFunc {

  TMS_Data::Work mnewWork;

  AddWorkFunc(TMS_Data::Work work):
    mnewWork(work)
  {};

  int operator()(std::string sessionKey) {
    int res = addWork(sessionKey,mnewWork);
    cout << "The work identifier is " << mnewWork.getWorkId() << endl;
    return res;
  }
};


int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string dietConfig;


  /********** EMF data ************/
  TMS_Data::Work newWork;

  /**************** Callback functions *************/

  boost::function1<void,std::string> fapp(boost::bind(&TMS_Data::Work::setApplicationId,boost::ref(newWork),_1));
  boost::function1<void,std::string> fsub( boost::bind(&TMS_Data::Work::setSubject,boost::ref(newWork),_1));
  boost::function1<void,int> fpri( boost::bind(&TMS_Data::Work::setPriority,boost::ref(newWork),_1));
  boost::function1<void,std::string> fown( boost::bind(&TMS_Data::Work::setOwner,boost::ref(newWork),_1));
  boost::function1<void,int> fest( boost::bind(&TMS_Data::Work::setEstimatedHour,boost::ref(newWork),_1));
  boost::function1<void,std::string> fdesc( boost::bind(&TMS_Data::Work::setDescription,boost::ref(newWork),_1));
  boost::function1<void,std::string> fpro( boost::bind(&TMS_Data::Work::setProjectId,boost::ref(newWork),_1));
  boost::function1<void,std::string> fmac( boost::bind(&TMS_Data::Work::setMachineId,boost::ref(newWork),_1));
  boost::function1<void,int> fcpu( boost::bind(&TMS_Data::Work::setNbCPU,boost::ref(newWork),_1));

  boost::shared_ptr<Options> opt= makeWorkOptions(av[0], dietConfig, fapp, fsub, fpri, fown, fest, fdesc, fpro, fmac, fcpu,0);

  CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt);
    return 0;
  }

  AddWorkFunc apiFunc(newWork);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main


