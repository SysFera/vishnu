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
 * \brief To build options for the VISHNU authentication (connect, reconnect)command
 * \param opt: Describes all allowed options for the VISHNU authentication
 * command
 * \param pgName: Represents the name of the command
 * \param userId : A callback for user identifier
 * \param req    : for required parameters, 0 optional paramter whereas 1 means
 * mandatory parameter
 * \param dietConfig      : Represents the VISHNU config file
 */


template <typename userIdType>

boost::shared_ptr<Options>
makeConnectOptions(std::string pgName,userIdType& userId,int req,std::string& dietConfig){

  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("userId,u",
           "represents the VISHNU user identifier",
           HIDDEN,
           userId,
           req);


  return opt;


}



#endif



