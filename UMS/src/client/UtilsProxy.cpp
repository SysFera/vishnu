/**
 * \file UtilsProxy.cpp
 * \brief This file contains the VISHNU UtilsProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include <string>
#include <iostream>
#include <fstream>

#include "UMSVishnuException.hpp"
#include "UtilsProxy.hpp"
#include "utilsClient.hpp"

/**
 * \fn UtilsProxy(char* cfg,
 *                int argc,
                  char** argv)
 * \param cfg The SYSFERA-DS configuration file
 * \param argc The number of arguments of the program
 * \briefargv The list of arguments
 */
UtilsProxy::UtilsProxy()
{
  mcfg  = NULL;
  margc = NULL;
  margv = NULL;
}

/**
 * \fn UtilsProxy(char* cfg,
 *                int argc,
                  char** argv)
 * \param cfg The SYSFERA-DS configuration file
 * \param argc The number of arguments of the program
 * \briefargv The list of arguments
 */
UtilsProxy::UtilsProxy(char* cfg, int argc, char** argv):
mcfg(cfg), margc(argc), margv(argv)
{
}

UtilsProxy::UtilsProxy(const std::string& filePath):
mfilePath(filePath)
{
  mcfg  = NULL;
  margc = NULL;
  margv = NULL;
}

/**
 * \brief Function to initialize the SYSFERA-DS configuration
 * \fn  int initialize()
 * \return 0 if success else 1 if error
 */
int
UtilsProxy::initialize() {
  if (diet_initialize(mcfg, margc, margv)) {
     return 1;
   }
 return 0;
}

/**
 * \brief Function to finalize
 * \fn  void finalize()
 * \return raises an exception on error
 */
void
UtilsProxy::finalize() {
 diet_finalize();
}
/**
 * \brief Function to initialize the database
 * \fn  int restore()
 * \return an error code
 */
int
UtilsProxy::restore() {

  int READSIZE = 1000;
  diet_profile_t* profile = NULL;
  //profile = diet_profile_alloc("restore", 0, 0, 0);
  std::ifstream file(mfilePath.c_str(), std::ios::in);
  if(!file) {
    std::cerr << "Failed to open sql script file" << std::endl;
    return -1;
  }
  // While all has not been read
  while (-1 != file.tellg()){
    char* tmp = new char[READSIZE];
    file.getline(tmp, READSIZE);
    if (strcmp(tmp, "")==0){
	break;
    }
    profile = diet_profile_alloc("restore", 0, 0, 0);
    std::cout << "Query sent :" << tmp << std::endl;
    //IN Parameters
    diet_string_set(diet_parameter(profile,0), tmp, DIET_VOLATILE);
    if(!diet_call(profile)){
      std::cerr << "Failed to restore database " << std::endl;
    }
  }
  return 0;
}

/**
 * \fn ~UtilsProxy()
 * \brief Destructor, raises an exception on error
 */
UtilsProxy::~UtilsProxy()
{
}

