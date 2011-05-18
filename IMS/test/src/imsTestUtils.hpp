#ifndef IMSTESTUTILS_HPP
#define IMSTESTUTILS_HPP

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_ims.hpp"

//TODO: renommer les fichiers imsTesUtils[.hpp et .cpp] en testUtils et les mettre dans core
//pour éviter la redondance de fonction avec tmsTestUtils inclure testUtils dans tmsautomTest.cpp

using namespace std;

struct VishnuConnexion{

VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co=UMS_Data::ConnectOptions());

~VishnuConnexion();

string getConnexion();

UMS_Data::Session msession;
UMS_Data::ConnectOptions mco;
std::string muid;
std::string mupwd;
bool open;
};


#endif
