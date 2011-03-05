#ifndef MACHINEUTILS__HH
#define MACHNINEUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

typedef  boost::function1<void,std::string> StringcallBackType;

boost::shared_ptr<Options> makeMachineOptions(std::string pgName,StringcallBackType& fName,std::string & dietConfig,
                                              StringcallBackType & fSite, StringcallBackType& fLanguage,
                                              std::string& sshPublicKeyPath, StringcallBackType& fMachineDescription,int type=0);





#endif



