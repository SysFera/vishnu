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

//#include "SessionServer.h"
/*#include "User.hpp"
#include "UMS_Data/User.hpp"
#include "UMS_Data/ConnectOptions.hpp"
#include "UMS_Data/ListUsers.hpp"
*/
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

class UserServer
{
private:
	UMS_Data::User muser;
	UMS_Data::ConnectOptions moptions;
	//Database* mdatabase;
	DbFactory factory;
	Database *mdatabaseVishnu;

private:
	bool isAdmin();
	bool checkLogin();
	bool checkPassword();
	int generatePassword();
public:
	UserServer(std::string userId, std::string password);
	UserServer(UMS_Data::User user);
	//void UserServer(SessionServer session);
	int add(UMS_Data::User user);
	int update(UMS_Data::User user);
	int deleteUser(UMS_Data::User user);
	int changePassword(std::string newPassword);
	int resetPassword(UMS_Data::User user);
	~UserServer();
	UMS_Data::User getData();
	bool exist();
	//static UMS_Data::ListUsers  list(SessionServer session, string userIdOptions);
};
#endif
