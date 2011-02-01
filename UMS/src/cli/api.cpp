
#include "api.h"

using namespace std;

int connect(string userId, string password, UMS_Data::ConnectOptions connectOpt, string sessionKey) {

  UserProxy userProxy(userId, password);
  SessionProxy sessionProxy;
  int res = sessionProxy.connect(userProxy, connectOpt);
  
 return res;
}
