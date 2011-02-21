#include "configureOptionUtils.hpp"
#include "Options.hh"
#include<iostream>


boost::shared_ptr<Options> makeConfigureOptions(std::string pgName,std::string& dietConfig,
		                                            StringcallBackType& fOptionName,StringcallBackType& fValue){

 boost::shared_ptr<Options> opt(new Options(pgName));


        opt->add("dietConfig,c",
                "The diet config file",
            ENV,
            dietConfig);



				opt->add("optionName",
                     "the name of an option",
                     HIDDEN,
                     fOptionName,
										 1);

				opt->setPosition("optionName",1);


				opt->add("value",
                       "The value of an option",
                        HIDDEN,
                        fValue,
												1);

				opt->setPosition("value",1);

    return opt;


}







