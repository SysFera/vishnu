#include "Options.hh"

#include "connectUtils.hpp"
#include "userUtils.hpp"
#include<iostream>
using namespace std;

boost::shared_ptr<Options> makeUserOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
		                                                   privilegeCallBackType & fPrivilege, StringcallBackType& fFirstname,
																										StringcallBackType& fLastname, StringcallBackType& fEmail, int type){


	boost::shared_ptr<Options> opt=makeConnectOptions(pgName,fUserId, dietConfig);


	Group_type group=CONFIG;

	if(type){// type =0 for "update function" and type=1 for "add function"

		group=group;
	}



	opt->add("privilege",
                     "the privilege of the user (admin or simple user)",
                     group,
                     fPrivilege,
										 type);


	if(type){
        opt->setPosition("privilege",1);
	}


	opt->setPosition("userId",1);

	opt->add("firstname",
                        "The firstname of the user",
                        group,
                        fFirstname,
												type);



	if(type){

		opt->setPosition("firstname",1);

	}


	opt->add("lastname",
                        "The lastname of the user",
                        group,
                        fLastname,
												type);

	if(type){
        opt->setPosition("lastname",1);

	}


	opt->add("email",
                        "The email of the user",
                        group,
                        fEmail,
												type);

	if(type){
        opt->setPosition("email",1);

	}



		return opt;



}






