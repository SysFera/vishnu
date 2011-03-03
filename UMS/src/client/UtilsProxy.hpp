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
 * \brief The class with the utilitaries function (not in the cli api)
 * \class UtilsProxy
 */
class UtilsProxy
{

  public:

    /**
     * \fn  UtilsProxy() 
     * \brief Constructor, default constructor
     */
    UtilsProxy();

    /**
     * \fn UtilsProxy(char* cfg,
     *                int argc,
     *                char** argv)
     * \param cfg The SYSFERA-DS configuration file
     * \param argc The number of arguments of the program
     * \param argv The list of arguments
     * \brief Constructor
     */
    UtilsProxy(char* cfg, int argc, char** argv);

    /**
     * \fn UtilsProxy(const std::string& filePath)
     * \param filePath The file used by the restore function 
     * \brief Constructor
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
