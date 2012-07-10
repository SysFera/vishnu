/**
* \file OptionValueServer.cpp
* \brief This file implements the Class which manipulates VISHNU options on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/
#include "OptionValueServer.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"

/**
* \brief Constructor
* \fn OptionValueServer()
*/

OptionValueServer::OptionValueServer() {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  moptionValue = NULL;
  msessionServer = NULL;
}
/**
* \brief Constructor
* \fn OptionValueServer(UMS_Data::OptionValue optionvalue)
* \param optionvalue the option data structure
* \param sessionServer The object to manipulate session
*/
OptionValueServer::OptionValueServer(UMS_Data::OptionValue*& optionvalue, SessionServer session):
moptionValue(optionvalue) , msessionServer(&session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}
/**
* \brief Function to configure options on the database
* \fn    int configureOption()
* \param defaultOptions the flag to get data from defauts options table
* \return  raises an exception on error
*/
int
OptionValueServer::configureOption(bool defaultOptions) {
  std::string sqlCommand;
  std::string numoptionid;
  std::string optionu_numoptionid;
  std::string numuserid;

  UserServer userServer = UserServer(SessionServer(msessionServer->getData().getSessionKey()));
  userServer.init();
  //if the user exists
  if (userServer.exist()) {
    //if the option exists
    if (getAttribut("where description='"+moptionValue->getOptionName()+"'", "numoptionid", true).size() != 0) {
      //if the value of this option is correct
      if (isCorrectValue()) {
        //if the default table is used
        if (defaultOptions) {
          //if the user is an admin
          if (userServer.isAdmin()){
            sqlCommand = "UPDATE optionu set defaultvalue="+moptionValue->getValue()+"\
            where description='"+moptionValue->getOptionName()+"'";
            mdatabaseVishnu->process(sqlCommand.c_str());
          } //END if the user is an admin
          else {
            UMSVishnuException e (ERRCODE_NO_ADMIN);
            throw e;
          }
        } //END if the default table is used
        else {
          //To get the database number of the userid
          numuserid = userServer.getAttribut("where"
          " userid='"+userServer.getData().getUserId()+"'and pwd='"+userServer.getData().getPassword()+"'");

          //To get the database number of the default option
          numoptionid = getAttribut("where "
          "description='"+moptionValue->getOptionName()+"'", "numoptionid", true);

          if (numoptionid.empty()) {
            UMSVishnuException e (ERRCODE_UNKNOWN_OPTION, moptionValue->getOptionName());
            throw e;
          }

          //To check if this option is already defined for the user
          optionu_numoptionid = getAttribut("where "
          "users_numuserid="+numuserid+" and optionu_numoptionid="+numoptionid);

          //if the option has not already defined for the user
          if (optionu_numoptionid.size() == 0) {
            sqlCommand = "insert into optionvalue (users_numuserid, optionu_numoptionid, value) values "
            " ("+numuserid+",'"+
            getAttribut("where description='"+moptionValue->getOptionName()+"'", "numoptionid", true)
            +"','"+moptionValue->getValue()+"')";
          } //End if the option has not already defined for the user
          else {
            sqlCommand = "UPDATE optionvalue set value="+moptionValue->getValue()+" "
            "where optionu_numoptionid="+numoptionid+" and users_numuserid="+numuserid;
          }
          mdatabaseVishnu->process(sqlCommand.c_str());
        }
      } //END if the value of this option is correct
      else {
        //if the option is VISHNU_CLOSE_POLICY
        if (moptionValue->getOptionName().compare(CLOSEPOLICY_OPT) == 0) {
          UMSVishnuException e (ERRCODE_UNKNOWN_CLOSURE_MODE);
          throw e;
        }
        else if (moptionValue->getOptionName().compare(TRANSFERCMD_OPT) == 0) {
          UMSVishnuException e (ERRCODE_INCORRECT_TRANSFER_CMD);
          throw e;
        }
        else {
          UMSVishnuException e (ERRCODE_INCORRECT_TIMEOUT);
          throw e;
        }
      }
    } //END if the option exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_OPTION);
      throw e;
    }
  }//END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}
/**
* \brief Destructor
* \fn  ~OptionValueServer()
*/
OptionValueServer::~OptionValueServer() {
}
/**
* \brief Function to get option data structure
* \fn UMS_Data::OptionValue getData()
* \return  The user data structure
*/
UMS_Data::OptionValue*
OptionValueServer::getData() {
  return moptionValue;
}
/**
* \brief Function to get option information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname, defaultOptions);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \param defaultOptions the flag to get data from defauts options table
* \return the value of the attribut or empty string if no results
*/
std::string
OptionValueServer::getAttribut(std::string condition, std::string attrname, bool defaultOptions) {
  std::string sqlCommand;

  if (defaultOptions) {
    sqlCommand = "SELECT "+attrname+" FROM optionu "+condition;
  }
  else {
    sqlCommand = "SELECT "+attrname+" FROM optionvalue "+condition;
  }
#ifdef USE_SOCI_ADVANCED
  std::string ret;
  SOCISession session = mdatabaseVishnu->getSingleSession();
  session.execute(sqlCommand).into(ret);
  mdatabaseVishnu->releaseSingleSession(session);
  return ret;
#else
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
#endif
}
/**
* \brief Function to get user option value
* \param numuserId The database number id of the user
* \param nameInfo the name of the option
*/
int
OptionValueServer::getOptionValueForUser(std::string numuserId, std::string optionName) {

  std::string optionId = getAttribut("where description='"+ optionName +"'", "numoptionid", true);
  if (optionId.empty()) {
     throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION, optionName);
  }
  std::string optionValue = getAttribut ("where users_numuserid='"+numuserId+"' and optionu_numoptionid=" + optionId);
  //If the option value is not defined for the user
  if (optionValue.size() == 0) {
    // get the default value of the option
    optionValue = getAttribut("where description='"+optionName+"'", "defaultvalue", true);
  }
  return convertToInt(optionValue);
}

/**
* \brief Function to check the value for a specific option
* \fn bool isCorrectValue()
* \return  true if the option value is correct else false
*/
bool
OptionValueServer::isCorrectValue() {

bool iscorrect = false;
int value = convertToInt(moptionValue->getValue());

  //if the option is VISHNU_CLOSE_POLICY
  if (moptionValue->getOptionName().compare(CLOSEPOLICY_OPT) == 0) {
      return ((value == 1) || (value == 2));
  }

  //if the option is VISHNU_TIMEOUT
  if (moptionValue->getOptionName().compare(TIMEOUT_OPT) == 0) {
      return (value > 0) ;
  }
  //if the option is VISHNU_TRANSFER_CMD
  if (moptionValue->getOptionName().compare(TRANSFERCMD_OPT) == 0) {
      return ( ( value==0)|| ( value==1)  ) ;
  }
  return iscorrect;
}
