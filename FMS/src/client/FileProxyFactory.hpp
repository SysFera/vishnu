/**
 * \file FileProxyFactory.hpp
 * This file declares a factory class to build a file object proxy
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \author Ibrahima Cisse (irahima.cisse@sysfera.com)
 */




#ifndef FILEPROXYFACTORY_HH
#define FILEPROXYFACTORY_HH

#include <string>
#include "SessionProxy.hpp"
#include "FileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "RemoteFileProxy.hpp"

/**
 * \brief  A factory class for the FileProxys classes.
 */

class FileProxyFactory {

  public:
    
    /**
     * \brief The factory function
     * \param sessionProxy the session proxy object
     * \param path the path of the file to build
     * \param user the owner of the file
     * \return  a new file proxy object
     */
  static FileProxy* getFileProxy(const SessionProxy& sessionProxy,const std::string& path,
                             const std::string& user="");
  
};

#endif
