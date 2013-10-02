/**
 * \file defineIdentifierUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU define
 * identifier command
 * \author Daouda Traoré (daouda.traore@sysfera.com)
 */



#ifndef DEFINEIDENTIFIER_HH
#define DEFINEIDENTIFIER_HH


#include<iostream>
#include<string>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"

#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "CLICmd.hpp"

class Options;


/**
 * \brief To build options for the VISHNU copy of file command
 * \param pgName The name of the command
 * \param configFile Represents the VISHNU config file
 * \param newFormat Represents the new format to use
 * \return The built option
 */
boost::shared_ptr<Options>
makeDefineIdentifierOptions(std::string pgName,
                            std::string& configFile,
                            std::string& newFormat) {
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);

  // All cli obligatory parameters
  opt->add("format,f",
      "The new format to use",
      HIDDEN,
      newFormat,1);
  opt->setPosition("format",1);

  return opt;
}

/**
 * \brief  A define type
 */

typedef enum{
  JOB,
  MACHINE,
  TRANSFER,
  USER,
  AUTH,
  WORK
} DefineIdentifierType;


/**
 * \brief A functor to handle define identifier api function
 */

template<DefineIdentifierType defineIdentifierType>
struct DefineIdentifierFunc {
  /**
   * \brief The new format
   */
  std::string mnewFormat;

  /**
   * \brief Constructor with parameters
   * \param newFormat The new format
   */
  DefineIdentifierFunc(const std::string& newFormat):mnewFormat(newFormat){};

  /**
   * \brief () operator
   * \param sessionKey The session key
   * \return 0 if it succeeds or an error code otherwise
   */
  int operator()(const std::string& sessionKey) {

    int res = -1;
    switch(defineIdentifierType) {
      case JOB:
        res = vishnu::defineJobIdentifier(sessionKey, mnewFormat);
        break;
      case MACHINE:
        res = vishnu::defineMachineIdentifier(sessionKey, mnewFormat);
        break;
      case TRANSFER:
        res = vishnu::defineTransferIdentifier(sessionKey, mnewFormat);
        break;
      case USER:
        res = vishnu::defineUserIdentifier(sessionKey, mnewFormat);
        break;
      case AUTH:
        res = vishnu::defineAuthIdentifier(sessionKey, mnewFormat);
        break;
      case WORK:
        res = vishnu::defineWorkIdentifier(sessionKey, mnewFormat);
        break;
      default:
        break;
    }

    return res;
  }
};


#endif
