#include <string>

#ifndef __WIN32__
#include <unistd.h>
#include <pwd.h>
#else
#include <Lmcons.h>
#endif

#include <sys/types.h>

#include "SessionProxy.hpp"

#include "FileProxy.hpp"
#include "LocalFileProxy.hpp"
#include "RemoteFileProxy.hpp"
#include "FileProxyFactory.hpp"

using namespace std;


FileProxy* FileProxyFactory::getFileProxy(const SessionProxy& sessionProxy,const string& path,
                                 const string& user) {
  string distUser(user);
  if (FileProxy::extHost(path)=="localhost"){
    return new LocalFileProxy(sessionProxy,path);
  }
 
  if (user=="") {
#ifndef __WIN32__
    uid_t uid = getuid();
    struct passwd*  pw = getpwuid(uid);
    distUser = pw->pw_name;
#else
    DWORD cchBuff = 256; // size of user name
    LPTSTR lpszSystemInfo; 
    char tchBuffer[UNLEN + 1]; // buffer for expanded string
    lpszSystemInfo = tchBuffer;
    GetUserName(lpszSystemInfo, &cchBuff);
    distUser = lpszSystemInfo;
#endif  
  }
  return new RemoteFileProxy(sessionProxy,path, distUser);
}

