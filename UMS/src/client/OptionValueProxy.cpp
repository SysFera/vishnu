#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "utilsClient.hpp"
#include "OptionValueProxy.hpp"


OptionValueProxy::OptionValueProxy(const UMS_Data::OptionValue& optionValue, const SessionProxy& session):
moptionValue(optionValue), msessionProxy(session)
{
}
 
int OptionValueProxy::setOptionValue(bool defaultValue)
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string optionValueToString;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   if(defaultValue) profile = diet_profile_alloc("optionValueSetDefault", 1, 1, 2);
   else profile = diet_profile_alloc("optionValueSet", 1, 1, 2);
   sessionKey = msessionProxy.getSessionKey();

   const char* name = "optionValueSet";
   ::ecorecpp::serializer::serializer _ser(name);
   optionValueToString =  _ser.serialize(const_cast<UMS_Data::OptionValue_ptr>(&moptionValue));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+sessionKey;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(optionValueToString.c_str()), DIET_VOLATILE)) {
      msg += "with optionValueToString parameter "+optionValueToString;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
         msg += " by receiving errorInfo message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
       }
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }

   /*To check the receiving message error*/
    checkErrorMsg(errorInfo);

  return 0;
}

int OptionValueProxy::configureOption() 
{
  return setOptionValue(false);
}
 
int OptionValueProxy::configureDefaultOption()
{
  return setOptionValue();
}
 
UMS_Data::OptionValue OptionValueProxy::getData()
{
  return moptionValue;
}
 
SessionProxy OptionValueProxy::getSessionProxy()
{
  return msessionProxy;
}

OptionValueProxy::~OptionValueProxy()
{
}
