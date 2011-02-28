#ifndef _LIST_OPTIONS_VALUES_SERVER_
#define _LIST_OPTIONS_VALUES_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListOptionsValues.hpp"
#include "ListOptOptions.hpp"


class ListOptionsValuesServer: public QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>
{

public:
  //Constructors
  ListOptionsValuesServer(const SessionServer session):
    QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>(session)
  {
    mcommandName = "vishnu_list_options";
  }
  ListOptionsValuesServer(UMS_Data::ListOptOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>(params, session)
  {
    mcommandName = "vishnu_list_options";
  }

  //To process options
  void processOptions(UserServer userServer, const UMS_Data::ListOptOptions_ptr& options, std::string& sqlRequest)
  {
      std::string sqlListofOptionValues = "SELECT description, value, userid from optionu, optionvalue, users\
                                     where optionu.numoptionid=optionvalue.optionu_numoptionid and\
                                     optionvalue.users_numuserid=users.numuserid";

     size_t userIdSize = options->getUserId().size();
     size_t nameSize = options->getOptionName().size();
     bool listAllDefault = options->isListAllDeftValue();
     
     if ((!userServer.isAdmin()) && (userIdSize!=0)) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
     }

     if(!listAllDefault) {

       if(userIdSize==0) {
          sqlRequest = sqlListofOptionValues;
          addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
       } else {
          sqlRequest = sqlListofOptionValues;
          addOptionRequest("userid", options->getUserId(), sqlRequest);
       }

       if(nameSize!=0) {
          addOptionRequest("description", options->getOptionName(), sqlRequest);
       }
    }

  }

  //To list
  UMS_Data::ListOptionsValues* list()
  {
     DatabaseResult *ListofOptions;
     std::string sqlListofOptions = "SELECT description, defaultvalue from optionu";

     std::vector<std::string>::iterator ii;
     std::vector<std::string> results;
     UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
     mlistObject = ecoreFactory->createListOptionsValues();

  try {
      //Creation of the object user
      UserServer userServer = UserServer(msessionServer);
      userServer.init();
      //if the user exists
    if (userServer.exist()) {
        //To process options 
        processOptions(userServer, mparameters, sqlListofOptions);
  
        //To get the list of options values from the database
        ListofOptions = mdatabaseVishnu->getResult(sqlListofOptions.c_str());
        if (ListofOptions->getNbTuples() != 0){
          for (size_t i = 0; i < ListofOptions->getNbTuples(); ++i) {
            results.clear();
            results = ListofOptions->get(i);
            ii = results.begin();

              UMS_Data::OptionValue_ptr optionValue = ecoreFactory->createOptionValue();;
              optionValue->setOptionName(*ii);
              optionValue->setValue(*(++ii));
              mlistObject->getOptionValues().push_back(optionValue);
          }
       } 
    } else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
     }
  }
  catch (VishnuException& e) {
        throw;
  }

    return mlistObject;
  }

  std::string getCommandName() 
  {
    return mcommandName;
  }

  //Destructor
  ~ListOptionsValuesServer()
  {
  }

  private:

  std::string mcommandName;
};

#endif
