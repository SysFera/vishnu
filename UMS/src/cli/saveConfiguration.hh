#ifndef SAVECONFIGURATION_HH
#define SAVECONFIGURATION_HH

/**
 * \file saveConfiguration.hh
 * This file declares convenient include files for the VISHNU save configuration process
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hh"
#include "api_ums.h"
#include <iostream>
#include <iterator>
#include <fstream>



/*
 * \struct env_name_mapper
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */


const unsigned int SCPARAM=1;

struct env_name_mapper{

        std::string operator ()(std::string name){

                std::string result= "";

                if (name=="VISHNU_CONFIG_FILE") {

                        result="dietConfig";
                }

								if (name=="VISHNU_SESSION_KEY"){

									result="sessionKey";

								}


                return result;
        }
};

		






#endif
