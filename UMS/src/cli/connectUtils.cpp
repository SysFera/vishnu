#include "Options.hh"

#include "connectUtils.hpp"

boost::shared_ptr<Options> makeConnectOptions(std::string pgName,std::string& userId,int pos,std::string & dietConfig){


	boost::shared_ptr<Options> opt(new Options(pgName));



	opt->add("version,v",
        "print version message",
        GENERIC );

        opt->add("dietConfig,c",
                "The diet config file",
            ENV,
            dietConfig);

    opt->add("userId,u",
                "represents the VISHNU user identifier",
            HIDDEN,
            userId,
            1);

		opt->setPosition("userId",pos);



		return opt;



}






