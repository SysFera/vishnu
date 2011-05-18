#ifndef FILEPROXYFACTORY_HH
#define FILEPROXYFACTORY_HH

#include <string>
#include "SessionProxy.hpp"
#include "FileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "RemoteFileProxy.hpp"

/* A factory class for the FileProxys classes. */

class FileProxyFactory {

  public:
  static FileProxy* getFileProxy(const SessionProxy& sessionProxy,const std::string& path,
                             const std::string& user="");
  
};

#endif
