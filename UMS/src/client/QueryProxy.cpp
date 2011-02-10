#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "QueryProxy.h"


QueryProxy::QueryProxy(const QueryParameters& params, const SessionProxy& session)
{
}
 
template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::list()
{

 return 0;
}
 
QueryProxy::~QueryProxy()
{
}
