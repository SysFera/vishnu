#ifndef TMSTESTUTILS_HPP
#define TMSTESTUTILS_HPP

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"



struct VishnuConnexion{


//addUser();
//changePassword();
//addMachine();
  VishnuConnexion(const std::string& uid, const std::string& upwd, const UMS_Data::ConnectOptions& co=UMS_Data::ConnectOptions());


~VishnuConnexion();

  std::string getConnexion();



UMS_Data::Session msession;
UMS_Data::ConnectOptions mco;
std::string muid;
std::string mupwd;
bool open;
};

bool operator== (const TMS_Data::Job& lJob,const TMS_Data::Job& rJob );

std::string findValue(const std::string& content, const std::string& key);

std::string getFileContent(const std::string& filePath);

std::string generateTmpScript(std::string& batchtype, std::string scriptType);

#endif
