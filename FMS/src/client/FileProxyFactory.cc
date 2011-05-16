#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "FileProxy.hh"
#include "LocalFileProxy.hh"
#include "RemoteFileProxy.hh"
#include "FileProxyFactory.hh"

using namespace std;


FileProxy* FileProxyFactory::getFileProxy(const string& path,
                                 const string& user) {
  string distUser(user);
  if (FileProxy::extHost(path)=="localhost")
    return new LocalFileProxy(path);

  if (user=="") {
    uid_t uid = getuid();
    struct passwd*  pw = getpwuid(uid);
    distUser = pw->pw_name;
  }
  return new RemoteFileProxy(path, distUser);
}

