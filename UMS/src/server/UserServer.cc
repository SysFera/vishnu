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
	
	std::string sqlCommand("SELECT firstname FROM users where userid='");
	sqlCommand.append(muser.getUserId()+ "' and pwd='");
	sqlCommand.append(muser.getPassword()+"'");
	
	
	std::cout <<"SQL COMMAND:"<<sqlCommand;
        try {
	DatabaseResult* result = mdatabaseVishnu->getResult(sqlCommand.c_str());
	
	   std::cout << "Nb résulats:" << result->getNbTuples() << std::endl;
	    //std::cout<<result->getNbTuples();
	    if (result->getNbTuples() != 0) {
	      res = true;
	    } 
	    else {
	    res =  false;
	    }
	} catch (SystemException& e) {
	//std::cout << "Message generique <-> 1: " << e.getMsg()<<std::endl;
	//std::cout << "Details supplementaires 2: " << e.what() <<std::endl;  
	throw e;
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
 
/* 
UMS_Data::ListUsers  UserServer::list(SessionServer session, string userIdOptions)
{
	return 0;
}*/
