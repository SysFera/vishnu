#include "Options.hh"

#include "connectUtils.hpp"
#include "listMachineUtils.hpp"
#include<iostream>
using namespace std;

boost::shared_ptr<Options> makeListMachineOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
		                                                    StringcallBackType& fMachineId){

	boost::shared_ptr<Options> opt(new Options(pgName));

	opt->add("dietConfig,c",
                        "The diet config file",
                        ENV,
                        dietConfig);

        opt->add("userId,u",
						"An admin option for listing machines in which a specific user has a local configuration",
                        CONFIG,
                        fUserId);


				opt->add("machineId,i",
                        "An option for listing information about a  specific machine",
                        CONFIG,
                        fMachineId);


		return opt;



}






