#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UserServer.hh"
#include <exception>
#include "SystemException.hh"



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
 
bool UserServer::isAdmin()
{
	return false;
}
 
 
bool UserServer::exist()
{
  
	bool res = false;
	DatabaseResult* result;
	
	std::string sqlCommand("SELECT * FROM users where userid='");
	sqlCommand.append(muser.getUserId()+ "' and pwd='");
	sqlCommand.append(muser.getPassword()+"'");
	
	
	std::cout <<"SQL COMMAND:"<<sqlCommand;
        try {
	result = mdatabaseVishnu->getResult(sqlCommand.c_str());
	} catch (SystemException& e) {
	//std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	//std::cout << "Details supplementaires 2: " << e.what() <<std::endl;  
	throw e;
	}
	    std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
	    //std::cout<<result->getNbTuples();
	    if (result->getNbTuples() != 0) {
	      res = true;
	    } 
	    else {
	    res =  false;
	    }
	
	return res;
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

int UserServer::getId() {
  
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;
	//TODO: factoriser avec getId de MachineClientServer 
        //faire une fonction global qui prend le sql et qui renvoie  Id ou -1
	
	std::string sqlCommand("SELECT numuserid FROM users where userid='");
	sqlCommand.append(muser.getUserId()+ "' and pwd='");
	sqlCommand.append(muser.getPassword()+"'");
	
	std::cout <<"SQL COMMAND:"<<sqlCommand;
        try {
	result = mdatabaseVishnu->getResult(sqlCommand.c_str());
	} catch (SystemException& e) {
	throw e;
	}
	
	if (result->getNbTuples() != 0) {
	     // res = true;
	     result->print();
	     std::vector<std::string> tmp = result->get(0);
	  
	  ii=tmp.begin();
	  std::istringstream str(*ii);
	  int value;
	  str>>value;
	  std::cout << "Value: "<< value;
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
/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
