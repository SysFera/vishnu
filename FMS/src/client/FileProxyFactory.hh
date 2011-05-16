#ifndef FILEPROXYFACTORY_HH
#define FILEPROXYFACTORY_HH

#include <string>

#include "FileProxy.hh"
#include "LocalFileProxy.hh"
#include "RemoteFileProxy.hh"

/* A factory class for the FileProxys classes. */

class FileProxyFactory {

  public:
  static FileProxy* getFileProxy(const std::string& path,
                             const std::string& user="");
  
};

#endif
