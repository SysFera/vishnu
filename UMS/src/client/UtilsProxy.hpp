/**
 * \file UtilsProxy.hpp
 * \brief This file contains the VISHNU UtilsProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _UTILS_PROXY_H_
#define _UTILS_PROXY_H_

#include <string>

/**
 * \brief The class with the utilitaries function (not in the cli api)
 * \class UtilsProxy
 */
class UtilsProxy {
public:
  /**
   * \brief Constructor, default constructor
   */
  UtilsProxy();

  /**
   * \brief Constructor
   * \param cfg The SYSFERA-DS configuration file
   * \param argc The number of arguments of the program
   * \param argv The list of arguments
   */
  UtilsProxy(char* cfg, int argc, char** argv);

  /**
   * \brief Constructor
   * \param filePath The file used by the restore function
   */
  UtilsProxy(const std::string& filePath);

  /**
   * \brief Function to initialize the SYSFERA-DS configuration
   * \return 0 if success else 1 if error
   */
  int
  initialize();

  /**
   * Destructor
   */
  ~UtilsProxy();

private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
   * \brief The name of the Sysfera-DS configuration file
   */
  char* mcfg;
  /**
   * \brief The number of arguments used Sysfera-DS
   */
  int margc;
  /**
   * \brief The list of arguments used Sysfera-DS
   */
  char** margv;
  /**
   * \brief The file used by the restore function
   */
  std::string mfilePath;
};
#endif
