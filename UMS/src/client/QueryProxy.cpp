#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "QueryProxy.h"


void QueryProxy::QueryProxy(QueryParameters params, SessionProxy session)
{
}
 
template <class QueryParameters, class ListObject>
 ListObject QueryProxy<QueryParameters, ListObject>::list()
{
	return 0;
}
 
void QueryProxy::~QueryProxy()
{
}
