#ifndef USERUTILS__HH
#define USERUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

typedef  boost::function1<void,std::string> StringcallBackType;
typedef boost::function1<void,UMS_Data::PrivilegeType> privilegeCallBackType;

boost::shared_ptr<Options> makeUserOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
                                                   privilegeCallBackType & fPrivilege, StringcallBackType& fFirstname,
                                                   StringcallBackType& fLastName, StringcallBackType & fEmail, int type=0);






#endif



