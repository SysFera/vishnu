#ifndef FILEPROXYFACTORY_HH
#define FILEPROXYFACTORY_HH

#include <string>
#include "SessionProxy.hpp"
#include "FileProxy.hh"
#include "LocalFileProxy.hh"
#include "RemoteFileProxy.hh"

/* A factory class for the FileProxys classes. */

class FileProxyFactory {

  public:
  static FileProxy* getFileProxy(const SessionProxy& sessionProxy,const std::string& path,
                             const std::string& user="");
  
};

#endif
