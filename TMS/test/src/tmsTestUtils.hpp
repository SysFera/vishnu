#ifndef TMSTESTUTILS_HPP
#define TMSTESTUTILS_HPP

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"

using namespace std;

struct VishnuConnexion{


//addUser();
//changePassword();
//addMachine();
VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co=UMS_Data::ConnectOptions());


~VishnuConnexion();

string getConnexion();



UMS_Data::Session msession;
UMS_Data::ConnectOptions mco;
std::string muid;
std::string mupwd;
bool open;
};

bool operator== (const Job& lJob,const Job& rJob );

std::string findValue(const std::string& content, const std::string& key);

#endif
