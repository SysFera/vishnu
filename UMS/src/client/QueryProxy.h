#ifndef QUERY_PROXY_H
#define QUERY_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "UMS_Data/Session.h"

template <class QueryParameters, class ListObject>
class QueryProxy
{
private:
	QueryParameters parameters;
	UMS_Data::Session session;
	SessionProxy sessionProxy;
 
public:
	void QueryProxy(QueryParameters params, SessionProxy session);
	ListObject list();
	void ~QueryProxy();
};
#endif
