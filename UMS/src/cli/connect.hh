#ifndef CONNECT_HH
#define CONNECT_HH

/**
 * \file connect.hh
 * This file defines convenients functions for the VISHNU connexion process
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hh"
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

struct env_name_mapper{

	std::string operator ()(std::string name){

		std::string result= "";
		
		if (name=="VISHNU_TRANSFER_CMD") {

			result="trCmd";
		}

		return result;
	}
};




#endif
