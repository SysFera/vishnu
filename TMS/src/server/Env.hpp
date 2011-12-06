/**
 * \file Env.hpp
 * \brief This file contains the VISHNU Env class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date December 2011
 */

#ifndef _ENV_H
#define _ENV_H

#include <sstream>
#include "utilVishnu.hpp"

/**
 * \class Env
 * \brief Env class implementation
 */
class Env
{

public:
  /**
   * \param batchType The batch scheduler type
   * \brief Constructor
   */
  Env(const BatchType& batchType);

  /*
  */
  void replaceAllOccurences(std::string& scriptContent,
      const std::string& oldValue,
      const std::string& newValue);

  /*
  */
  void replaceEnvVariables(std::string& scriptContent);

private:

  void getTorqueNumNodes(std::string& scriptContent);
  /**
   * \brief The type of the batch scheduler 
   */
  BatchType mbatchType;

};

#endif
