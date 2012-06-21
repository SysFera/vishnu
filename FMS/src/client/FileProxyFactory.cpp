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


FileProxy* FileProxyFactory::getFileProxy(const SessionProxy& sessionProxy,const string& path,
                                 const string& user) {
  string distUser(user);
  if (FileProxy::extHost(path)=="localhost" || !vishnu::isNotIP(FileProxy::extHost(path))){
    return new LocalFileProxy(sessionProxy,path);
  }

  if (user=="") {
    uid_t uid = getuid();
    struct passwd*  pw = getpwuid(uid);
    distUser = pw->pw_name;
  }
  return new RemoteFileProxy(sessionProxy,path, distUser);
}

