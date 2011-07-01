/**
 * \file fileTrnsferUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU file
 * transfer
 * command
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */



#ifndef FILETRANSFERUTILS_HH
#define FILETRANSFERUTILS_HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "FMS_Data.hpp"


class Options;



/**
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */
struct FMS_env_name_mapper{

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

                if (name=="VISHNU_TRANSFER_CMD") {

                        result="trCommand";
                }

		return result;
	}
};




/**
 * \brief To build options for the VISHNU copy of file command
 * \param pgName: The name of the command
 * \param dietConfig: Represents the VISHNU config file
 * \param trCmdStr: The command to use to perform file transfer
 * \param src: The source file to copy following the pattern [host:]file path
 * \param dest: The path of the destination file
 */
boost::shared_ptr<Options>
makeTransferCommandOptions(string pgName, 
    string& dietConfig,
    string& trCmdStr,
    string& src,
    string& dest){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);
  
  opt->add("trCommand,t",
      "The command to use to perform file transfer. The different values  are:\n"
      "O or scp: for SCP transfer\n"
      "1 or rsync: for RSYNC transfer\n", 
      ENV,
      trCmdStr);

  opt->add("src, s",
      "The source file to copy following the pattern [host:]file path.",
      HIDDEN,
      src,1);
  opt->setPosition("src",1);

  opt->add("dest, d",
      "The path of the destination file.",
      HIDDEN,
      dest,1);
  opt->setPosition("dest",1);

  return opt;
}

#endif



