#include "Options.hh"

#include "connectUtils.hpp"
#include "listMachineUtils.hpp"
#include<iostream>
using namespace std;

boost::shared_ptr<Options> makeListSessionOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
                                                    StringcallBackType& fMachineId, StringcallBackType& fSessionId,
                                                  StatuscallBackType& fStatus,ClosePolicycallBackType& fClosePolicy,
                                                  IntcallBackType& fSessionInactivityDelay,
                                                   LongcallBackType& fStartDateOption, LongcallBackType& fEndDateOption){


	boost::shared_ptr<Options> opt(new Options(pgName));









		return opt;



}






