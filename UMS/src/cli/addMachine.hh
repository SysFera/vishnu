#ifndef ADDMACHINE_HH
#define ADDMACHINE_HH

/**
 * \file addMachine.hh
 * This file defines convenients functions for the VISHNU add machine  process
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hh"
#include "api_ums.hpp"
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>


//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


/*
 * \struct env_name_mapper
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */

const unsigned int AMPARAM=3;


struct env_name_mapper{

	std::string operator ()(std::string name){

		std::string result= "";

                if (name=="VISHNU_CONFIG_FILE") {

                        result="dietConfig";
                }


		return result;
	}
};




#endif
