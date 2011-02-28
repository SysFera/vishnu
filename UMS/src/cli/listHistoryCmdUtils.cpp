#include "Options.hpp"

#include "listHistoryCmdUtils.hpp"
#include<iostream>
using namespace std;
boost::shared_ptr<Options> makeListHistoryCmdOptions(std::string pgName,StringcallBackType& fUserId,std::string & dietConfig,
                                                     StringcallBackType& fSessionId,
                                                     LongcallBackType& fStartDateOption, LongcallBackType& fEndDateOption){


  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("adminListOption,a",
           "is an admin option for listing all commands of all users ",
           CONFIG);

  opt->add("userId,u",
           "an admin option for listing commands launched\n"
           "by a specific user identified by his/her userId",
           CONFIG,
           fUserId);

  opt->add("sessionId,i",
           "lists all commands launched within a specific session",
           CONFIG,
           fSessionId);

  opt->add("startDateOption,s",
           "allows the user to organize the commands listed\n"
           "by providing the start date\n"
           "(the UNIX timestamp  of the start date is used)",
           CONFIG,
           fStartDateOption);

  opt->add("endDateOption,e",
           "allows the user to organize the commands listed\n"
           "by providing the end date (the timestamp of the end date is used).\n"
           "By default, the end date is the current day",
           CONFIG,
           fEndDateOption);




  return opt;



}






