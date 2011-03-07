#ifndef COMMON_HPP
#define COMMON_HPP

/**
 * \file common.hpp
 * This file defines convenients include file for the all VISHNU command line
 * interface 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hpp"
#include "api_ums.hpp"
#include "cliError.hpp"
#include <iostream>
#include <iterator>
#include <fstream>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


/**
 * \struct env_name_mapper
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */


struct env_name_mapper{

/**
 * Mapper operator for environnement variable
 * \param name The environment variable name
 * \return The name used in the program
 */

	std::string operator ()(std::string name){

		std::string result= "";

                if (name=="VISHNU_CONFIG_FILE") {

                        result="dietConfig";
                }


		return result;
	}
};

const std::string dietErrorMsg="DIET initialization failed !";
const std::string currentSessionKeyMsg="the current sessionkey is: ";
const std::string requiredParamMsg="required parameter is missing";

#endif
