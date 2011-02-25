#ifndef RESETPASSWORD_HH
#define RESETPASSWORD_HH

/**
 * \file resetPassword.hh
 * This file declares convenient include files for the VISHNU reset password process
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "Options.hh"
#include "api_ums.hpp"
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


const unsigned int RPPARAM=1;

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
