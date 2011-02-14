#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "UMSVishnuException.hh"
#include "utilsClient.hpp"

#include "ConfigurationProxy.h"


ConfigurationProxy::ConfigurationProxy(const std::string& filePath, const SessionProxy& session):
mfilePath(filePath), msessionProxy(session)
{
  mconfiguration = NULL;
}
 
ConfigurationProxy::ConfigurationProxy(UMS_Data::Configuration* config, const SessionProxy& session):
mconfiguration(config), msessionProxy(session)
{
}
 
int ConfigurationProxy::save()
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   char* errorInfo;
   char* configurationInString;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("configurationSave", 0, 0, 2);
   sessionKey = msessionProxy.getSessionKey();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+sessionKey;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE);
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,1), &configurationInString, NULL)){
           msg += " by receiving configurationInString message";
           ERRMSG(msg.c_str());
           sendErrorMsg(msg);      
       }
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

   // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  // Parse the model
  ecorecpp::parser::parser parser;
  //To set the mconfiguration 
  mconfiguration = parser.load(configurationInString)->as< UMS_Data::Configuration >(); 
  
  //To save the configuration in the file
  std::ofstream ofile(mfilePath.c_str());
  ofile << configurationInString;
  ofile.close();
   
  return 0;
}
 
int ConfigurationProxy::restore(bool fromFile)
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   char* errorInfo;
   char* configurationInString;
   std::string msg = "call of function diet_string_set is rejected "; 
 
   if(fromFile) {
     size_t length;
     std::ifstream ifile(mfilePath.c_str()); 
 
     if(!ifile.is_open()) return 1;
 
     ifile.seekg(0, std::ios::end);
     length = ifile.tellg();
     ifile.seekg(0, std::ios::beg);

     configurationInString = new char[length];
     ifile.read(configurationInString, length);
     ifile.close();
   }
   else {
     const char* name = "ConfigurationRestore";
     ::ecorecpp::serializer::serializer _ser(name);
     configurationInString =  strdup((_ser.serialize(mconfiguration)).c_str());
   }
   profile = diet_profile_alloc("configurationRestore", 1, 1, 2);
   sessionKey = msessionProxy.getSessionKey();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+sessionKey;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), configurationInString, DIET_VOLATILE)) {
      msg += "with configurationInString parameter "+std::string(configurationInString);
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
  
   if(fromFile) free(configurationInString);

  return 0;
}

int ConfigurationProxy::restoreFromFile()
{
  return restore();
}
 
int ConfigurationProxy::restoreFromData()
{
  return restore(false);
}
 
UMS_Data::Configuration* ConfigurationProxy::getData()
{
 return mconfiguration;
}
 
SessionProxy ConfigurationProxy::getSessionProxy()
{
  return msessionProxy;
}

ConfigurationProxy::~ConfigurationProxy()
{
}
