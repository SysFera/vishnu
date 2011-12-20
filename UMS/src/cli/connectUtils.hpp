/**
 * \file connectUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU connexion
 * command
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */



#ifndef CONNECTUTILS__HH
#define CONNECTUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;



/**
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */
struct UMS_env_name_mapper{

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

                if (name=="VISHNU_CLOSE_POLICY") {

                        result="closePolicy";
                }

		return result;
	}
};




/**
 * \brief To build options for the VISHNU authentication (connect, reconnect)command
 * command
 * \param pgName: Represents the name of the command
 * \param userId : A callback for user identifier
 * \param req    : for required parameters, 0 optional paramter whereas 1 means
 * mandatory parameter
 * \param dietConfig      : Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */


template <typename userIdType>

boost::shared_ptr<Options>
makeConnectOptions(std::string pgName,userIdType& userId,int req,std::string& dietConfig, const Group_type& userIdGroup=HIDDEN){

  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("userId,u",
           "represents the VISHNU user identifier",
           userIdGroup,
           userId,
           req);


  return opt;


}


#endif



