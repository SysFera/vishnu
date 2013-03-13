/**
 * \file cmdArgs.hpp
 * This file defines the VISHNU FMS cli utility
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */

#ifndef CMD_ARGS
#define CMD_ARGS

#include <boost/bind.hpp>
#include <string>

namespace po = boost::program_options;


/**
 * \brief To build options for the VISHNU cmdArgs command
 * \param pgName : The name of the command
 * \param configFile: Represents the VISHNU config file
 * \return  The built command
 */
boost::shared_ptr<Options>
processOpt(std::string pgName,
           std::string& configFile);

#endif
