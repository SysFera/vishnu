#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserServer.hh"
#include <exception>
#include "SystemException.hh"


/*
int convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val); 
  str >> intValue;
  return static_cast<int> (intValue);
}
*/

UserServer::UserServer(std::string userId, std::string password) {
  muser.setUserId(userId);
  muser.setPassword(password);
  mdatabaseVishnu = factory.getDatabaseInstance();
}
/* 
UserServer::UserServer(UMS_Data::User user) {
  
}
 
void UserServer::UserServer(SessionServer session)
{
}
 
int UserServer::add(UMS_Data::User user)
{
	return 0;
}
 
int UserServer::update(UMS_Data::User user)
{
	return 0;
}
 
int UserServer::deleteUser(User user)
{
	return 0;
}
 
int UserServer::changePassword(string newPassword)
{
	return 0;
}
 
int UserServer::resetPassword(User user)
{
	return 0;
}
*/ 
UserServer::~UserServer()
{
}
 
UMS_Data::User UserServer::getData()
{
	return muser;
}
 
bool UserServer::isAdmin() {
  
  try {
         
      return 
      (
	 convertToInt (
	   getAttribut("where userid='"+muser.getUserId()+"'and \
           pwd='"+muser.getPassword()+"'", " privilege")
	   ) != 0
      );
 } 
 catch (SystemException& e) {
	throw e;
 }
	//return false;	
	//getAttribut("where userid='"+user.getData().getUserId()+"'and pwd='"+user.getData().getPassword()+"'", " privilege");
}
 
 
bool UserServer::exist() {
 try {
      return 
      (getAttribut("where \
      userid='"+muser.getUserId()+"'and pwd='"+muser.getPassword()+"'").size() != 0	
      );
 } 
 catch (SystemException& e) {
	throw e;
 } 	
}
 
bool UserServer::checkLogin()
{
	return false;
}
 
bool UserServer::checkPassword()
{
	return false;
}
 
int UserServer::generatePassword()
{
	return 0;
}

/*
int UserServer::getId() {
  
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;
  
  std::string sqlCommand("SELECT numuserid FROM users where userid='");
  sqlCommand.append(muser.getUserId()+ "' and pwd='");
  sqlCommand.append(muser.getPassword()+"'");
  std::cout <<"SQL COMMAND:"<<sqlCommand;
  
  try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
  throw e;
  }
	
  if (result->getNbTuples() != 0) {
      result->print();
	     std::vector<std::string> tmp = result->get(0);
	  
	  ii=tmp.begin();
	  std::istringstream str(*ii);
	  int value;
	  str>>value;
	  std::cout << "Value: "<< value << std::endl;
	  return value;
	    //      ii=tmp.begin();
	     //return 0;
	  //   return static_cast<int> (*ii);
	} 
	else {
	  return -1;
	    //res =  false;
	}
}
*/

std::string UserServer::getAttribut(std::string condition, std::string attrname) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 
 std::string sqlCommand("SELECT "+attrname+" FROM users "+condition);
 std::cout <<"SQL COMMAND:"<<sqlCommand;
   
  try {
  result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
  throw e;
  }
  
  if (result->getNbTuples() != 0) {
      result->print();
      std::vector<std::string> tmp = result->get(0);  
      ii=tmp.begin();
      /*std::istringstream str(*ii);
      str>>value;
	  std::cout << "Value: "<< value << std::endl;
	  return static_cast<int> (value);*/
      return (*ii);
   } 
   else {
	  return "";
	}
}

/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
