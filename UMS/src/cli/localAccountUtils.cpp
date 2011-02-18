#include "Options.hh"

#include "connectUtils.hpp"
#include "localAccountUtils.hpp"
#include<iostream>
using namespace std;
boost::shared_ptr<Options> makeLocalAccountOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
		                                               StringcallBackType & fMachineId, StringcallBackType& fAcLogin,
																									 StringcallBackType& fSshKeyPath, StringcallBackType & fHomeDirectory, int type){

	boost::shared_ptr<Options> opt=makeConnectOptions(pgName,fUserId, dietConfig);

	opt->setPosition("userId",1);

	Group_type group=CONFIG;

	if(type){// type =0 for "update function" and type=1 for "add function"

		group=HIDDEN;
	}



        opt->add("machineId",
                        "the identifier of the machine associated to the local user configuration",
                        HIDDEN,
                        fMachineId,
                        1);


					opt->setPosition("machineId",1);

				opt->add("acLogin,l",
                        "login of the user on the associated machine",
                        group,
                        fAcLogin,
                        type);

				if (type){
        opt->setPosition("acLogin",1);
				}

        opt->add("sshKeyPath,s",
                        "The path of the ssh key of the user on the associated machine",
                        group,
                        fSshKeyPath,
                        type);

				if (type){
        opt->setPosition("sshKeyPath",1);
				}

        opt->add("homeDirectory,d",
                        "The path of the home directory of the user on the associated machine",
                        group,
                        fHomeDirectory,
                        type);

				if (type){
        opt->setPosition("homeDirectory",1);
				}

		return opt;



}






