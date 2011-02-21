/**
 * \file UtilsProxy.hpp
 * \brief This file contains the VISHNU UtilsProxy class.
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#ifndef _UTILS_PROXY_H_
#define _UTILS_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

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
   * \fn UtilsProxy(const std::string filePath)
   */
  UtilsProxy(const std::string& filePath);
 
  /**
   * \brief Function to initialize the SYSFERA-DS configuration 
   * \fn  int initilialize()
   * \return 0 if success else -1 if error
   */
  int
  initilialize();
  /**
   * \brief Function to finalize 
   * \fn  void finalize()
   * \return raises an exception on error
   */
  int 
  finalize();
   /**
   * \brief Function to initialize the database 
   * \fn  void restore()
   * \return raises an exception on error
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
