#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <fstream>

#include "UMSVishnuException.hh"
#include "debug.hh"

#include "ConfigurationProxy.h"


ConfigurationProxy::ConfigurationProxy(const std::string& filePath, const SessionProxy& session):
mfilePath(filePath), msessionProxy(session)
{
}
 
ConfigurationProxy::ConfigurationProxy(const UMS_Data::Configuration& config, const SessionProxy& session):
mconfiguration(config), msessionProxy(session)
{
}
 
int ConfigurationProxy::save()
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   char* errorInfo;
   char* configurationInString;

   profile = diet_profile_alloc("configurationSave", 0, 0, 2);
   sessionKey = msessionProxy.getSessionKey();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,1), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,1), &configurationInString, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

   // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  // Parse the model
  ecorecpp::parser::parser parser;
  UMS_Data::Configuration_ptr config_ptr = parser.load(configurationInString)->as< UMS_Data::Configuration >();
   
  //To set the file path 
  mconfiguration.setFilePath(mfilePath);
  //To set the user list 
  for(int i = 0; i < config_ptr->getListConfUsers().size(); i++) {
      UMS_Data::User_ptr user = config_ptr->getListConfUsers().get(i);
      mconfiguration.getListConfUsers().push_back(user);
  }
  //To set the machine list 
  for(int i = 0; i < config_ptr->getListConfMachines().size(); i++) {
      UMS_Data::Machine_ptr machine = config_ptr->getListConfMachines().get(i);
      mconfiguration.getListConfMachines().push_back(machine);
  }
  //To set the LocalAccounts list 
  for(int i = 0; i < config_ptr->getListConfLocalAccounts().size(); i++) {
      UMS_Data::LocalAccount_ptr localAccount = config_ptr->getListConfLocalAccounts().get(i);
      mconfiguration.getListConfLocalAccounts().push_back(localAccount);
  }

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
     configurationInString =  strdup((_ser.serialize(&mconfiguration)).c_str());
   }
   profile = diet_profile_alloc("configurationRestore", 1, 1, 2);
   sessionKey = msessionProxy.getSessionKey();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }
   if(diet_string_set(diet_parameter(profile,1), configurationInString, DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   }
   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }
  
  
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
 
UMS_Data::Configuration ConfigurationProxy::getData()
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
