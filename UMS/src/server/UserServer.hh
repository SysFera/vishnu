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
class SessionServer;
#include "SessionServer.hh"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

class UserServer {

public:
        UserServer();
	UserServer(std::string userId, std::string password);
	UserServer(UMS_Data::User user);
	UserServer(SessionServer sessionServer);
	int add(std::string userSerialized);
	int update(UMS_Data::User user);
	int deleteUser(UMS_Data::User user);
	int changePassword(std::string newPassword);
	int resetPassword(UMS_Data::User user);
	~UserServer();
	UMS_Data::User getData();
	bool exist();
	bool isAdmin();
	bool isPwdStateOk();//return if the password state ok
	//int getId();
	//return the integer attribut of the user
	std::string getAttribut(std::string condition, std::string attrname = "numuserid");
	//static UMS_Data::ListUsers  list(SessionServer session, string userIdOptions);

private:
	UMS_Data::User muser;
	UMS_Data::ConnectOptions moptions;
	//Database* mdatabase;
	DbFactory factory;
	Database *mdatabaseVishnu;
	SessionServer* msessionServer;

private:
	bool checkLogin();
	bool checkPassword();
	int generatePassword();
	
};
#endif
