#ifndef _LOCAL_ACCOUNT_PROXY_H_
#define _LOCAL_ACCOUNT_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "LocalAccount.hpp"

class LocalAccountProxy
{
 
public:
  LocalAccountProxy(const UMS_Data::LocalAccount& localAccount, const SessionProxy& session);
  int add();
  int update();
  int deleteLocalAccount();
  SessionProxy getSessionProxy();
  ~LocalAccountProxy();

private:
  int _addLocalAccountInformation(bool isNewMachine=true);

  UMS_Data::LocalAccount mlocalAccount;
  SessionProxy msessionProxy;
};
#endif
