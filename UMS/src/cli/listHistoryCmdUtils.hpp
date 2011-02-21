#ifndef LISTHISTORYCMDUTILS__HH
#define LISTHISTORYCMDUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>
#include<boost/function.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

typedef  boost::function1<void,std::string> StringcallBackType;
typedef  boost::function1<void,UMS_Data::SessionCloseType> ClosePolicycallBackType;
typedef  boost::function1<void,UMS_Data::UMS_Data::StatusType> StatuscallBackType;
typedef  boost::function1<void,long> LongcallBackType;
typedef  boost::function1<void,int> IntcallBackType;

boost::shared_ptr<Options> makeListSessionOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
                                                     StringcallBackType& fSessionId,
																									StatuscallBackType& fStatus,ClosePolicycallBackType& fClosePolicy,
																									IntcallBackType& fSessionInactivityDelay,
																									 LongcallBackType& fStartDateOption, LongcallBackType& fEndDateOption);






#endif



