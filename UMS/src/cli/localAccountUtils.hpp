#ifndef LOCALACCOUNTUTILS__HH
#define LOCALACCOUNTUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

typedef  boost::function1<void,std::string> StringcallBackType;

boost::shared_ptr<Options> makeLocalAccountOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
                                                   StringcallBackType & fMachineId, StringcallBackType& fAcLogin,
                                                   StringcallBackType& fSshKeyPath, StringcallBackType & fHomeDirectory, int type=0);






#endif



