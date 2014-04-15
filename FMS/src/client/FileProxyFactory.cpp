#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "SessionProxy.hpp"

#include "FileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "RemoteFileProxy.hpp"
#include "FileProxyFactory.hpp"
#include "utilVishnu.hpp"

using namespace std;


FileProxy* FileProxyFactory::getFileProxy(const SessionProxy& sessionProxy,const string& path) {
  return new RemoteFileProxy(sessionProxy,path);
}

