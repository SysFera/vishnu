#ifndef LOCAL_ACCOUNT_PROXY_H
#define LOCAL_ACCOUNT_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "UMS_Data/LocalAccount.h"

class LocalAccountProxy
{
private:
	UMS_Data::LocalAccount localAccount;
	SessionProxy sessionProxy;
	SessionProxy sessionProxy;
 
public:
	void LocalAccountProxy(UMS_Data::LocalAccount localAccount, SessionProxy session);
	int add();
	int update();
	int delete();
	void ~LocalAccountProxy();
	SessionProxy getSessionProxy();
};
#endif
