/**
 * \file UtilsProxy.hpp
 * \brief This file contains the VISHNU UtilsProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _UTILS_PROXY_H_
#define _UTILS_PROXY_H_

#include "DIET_client.h"

/**
 * \class UtilsProxy
 * \brief UtilsProxy class implementation
 */
class UtilsProxy
{

public:

  /**
   * \fn UtilsProxy()
   */
  UtilsProxy();

  /**
   * \fn UtilsProxy(char* cfg,
   *                int argc,
                    char** argv)
   * \param cfg The SYSFERA-DS configuration file
   * \param argc The number of arguments of the program
   * \briefargv The list of arguments
   */
  UtilsProxy(char* cfg, int argc, char** argv);

  /**
   * \fn UtilsProxy(const std::string filePath, char* cfg, int argc, char** argv)
   */
  UtilsProxy(const std::string& filePath);

  /**
   * \brief Function to initialize the SYSFERA-DS configuration
   * \fn  int initialize()
   * \return 0 if success else 1 if error
   */
  int
  initialize();
  /**
   * \brief Function to finalize
   * \fn  void finalize()
   * \return raises an exception on error
   */
  void
  finalize();
   /**
   * \brief Function to initialize the database
   * \fn int restore()
   * \return an error code
   */
  int
  restore();

  ~UtilsProxy();

private:

 char* mcfg;
 int margc;
 char** margv;
 std::string mfilePath;

};
#endif
