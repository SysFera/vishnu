#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <string.h>

#include "SessionProxy.h"
#include "UserProxy.h"

#include "DIET_client.h"

SessionProxy::SessionProxy(std::string sessionKey)
{
}
 
SessionProxy::SessionProxy(UMS_Data::Session session)
{
}
 
SessionProxy::SessionProxy()
{
}
 
int SessionProxy::connect(UserProxy user, UMS_Data::ConnectOptions options)
{
  
  diet_profile_t* profile = diet_profile_alloc("connect", 0, 1, 2);

  UserProxy userProxy(user.getData().getUserId(), user.getData().getPassword());
  
  diet_string_set(diet_parameter(profile,0), strdup((userProxy.getData().getUserId()).c_str()), DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,1), strdup((userProxy.getData().getUserId()).c_str()), DIET_VOLATILE);


  return 0;
}
 
int SessionProxy::reconnect(UserProxy user)
{
	return 0;
}
 
int SessionProxy::close()
{
	return 0;
}
 
UMS_Data::Session SessionProxy::getData()
{
	return UMS_Data::Session();
}
 
SessionProxy::~SessionProxy()
{
}
