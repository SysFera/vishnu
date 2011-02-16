#ifndef _QUERY_PROXY_H_
#define _QUERY_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "utilsClient.hpp"

#include "SessionProxy.hpp"
#include "Session.hpp"
#include "ListCmdOptions.hpp"
#include "ListCommands.hpp"
#include "ListLocalAccOptions.hpp"
#include "ListLocalAccounts.hpp"
#include "ListMachineOptions.hpp"
#include "ListMachines.hpp"
#include "ListOptionsValues.hpp"
#include "ListOptOptions.hpp"
#include "ListSessionOptions.hpp"
#include "ListSessions.hpp"
#include "ListUsers.hpp"

template <class QueryParameters, class ListObject>
class QueryProxy
{
 
public:
  QueryProxy(const QueryParameters& params, const SessionProxy& session, const std::string& serviceName);
  ListObject* list();
  ListObject* listWithParamsString();
  ~QueryProxy();

private:
  typedef ListObject* ListObject_ptr;
  typedef QueryParameters* QueryParameters_ptr;
  QueryParameters mparameters;
  std::string mserviceName;
  SessionProxy msessionProxy;
  ListObject* mlistObject;

};

template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::QueryProxy(const QueryParameters& params, const SessionProxy& session, 
                                                    const std::string& serviceName):
  mparameters(params), msessionProxy(session), mserviceName(serviceName)
{
  mlistObject = NULL;
}

template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::list()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string queryParmetersToString;
  char* listObjectInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";
  
  profile = diet_profile_alloc(mserviceName.c_str(), 1, 1, 3);
  sessionKey = msessionProxy.getSessionKey();

  const char* name = mserviceName.c_str();
  ::ecorecpp::serializer::serializer _ser(name);
  queryParmetersToString =  _ser.serialize(const_cast<QueryParameters_ptr>(&mparameters));
   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(queryParmetersToString.c_str()), DIET_VOLATILE)) {
     msg += "with queryParmetersToString parameter "+queryParmetersToString;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
   diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &listObjectInString, NULL)){
         msg += " by receiving listObjectInString message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
       }
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
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

   //CREATE the ListOject
   UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
   ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
   ecorecpp::parser::parser parser;
   mlistObject = parser.load(listObjectInString)->as< ListObject >();

 return mlistObject;
}

template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::listWithParamsString()
{
  diet_profile_t* profile = NULL;
  char* listObjectInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc(mserviceName.c_str(), 1, 1, 3);
   
  //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy.getSessionKey()).c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+msessionProxy.getSessionKey();
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(mparameters.c_str()), DIET_VOLATILE)) {
      msg += "with mparameters parameter "+mparameters;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
   diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &listObjectInString, NULL)){
         msg += " by receiving listObjectInString message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
       }
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
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

   //CREATE the ListOject
   UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
   ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
   ecorecpp::parser::parser parser;
   mlistObject = parser.load(listObjectInString)->as< ListObject >();

 return mlistObject;

}

template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::~QueryProxy()
{
}


#endif
