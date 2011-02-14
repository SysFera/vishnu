#ifndef USER_SERVER_H
#define USER_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include "POSTGREDatabase.hh"
#include "DbFactory.hh"
#include "DatabaseResult.hh"

//#include "SessionServer.hh"
/*#include "User.hpp"
#include "UMS_Data/User.hpp"
#include "UMS_Data/ConnectOptions.hpp"
#include "UMS_Data/ListUsers.hpp"
*/
#include "utilServer.hh"
using namespace utilServer;

//extern std::string utilServer::mvishnuid;
//extern utilServer::m ;
//class ServerUMS;
class SessionServer;
#include "SessionServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

class UserServer {

public:
        UserServer();
	UserServer(std::string userId, std::string password);
	UserServer(const UMS_Data::User& user);
	UserServer(SessionServer sessionServer);
	int 
	add(UMS_Data::User*& user);
	int 
	update(UMS_Data::User* user);
	int 
	deleteUser(UMS_Data::User user);
	int 
	changePassword(std::string newPassword);
	int 
	resetPassword(UMS_Data::User user);
	~UserServer();
	UMS_Data::User 
	getData();
	void 
	init();
	bool 
	exist(bool flagForChangePwd = false);
	bool 
	isAdmin();
	bool 
	isAttributOk(std::string attributName, int valueOk);//return if the password state ok
	//int getId();
	//return the integer attribut of the user
	std::string getAttribut(std::string condition, std::string attrname = "numuserid");
	//static UMS_Data::ListUsers  list(SessionServer session, string userIdOptions);
	bool 
	existuserId(std::string userId);
	
	
private:
	UMS_Data::User muser;
	UMS_Data::ConnectOptions moptions;
	//Database* mdatabase;
	DbFactory factory;
	Database* mdatabaseVishnu;
	SessionServer* msessionServer;

private:
	bool 
	checkPassword();
	std::string 
	generatePassword(std::string lastname, std::string firstname);
	
};
#endif
