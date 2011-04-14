#ifndef COMMON_HPP
#define COMMON_HPP

/**
 * \file common.hpp
 * \brief This file defines convenients include file for the all VISHNU command line
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
 * \brief Contains a generic error message for required parameter missed
 */
const std::string requiredParamMsg="required parameter is missing";

#endif
