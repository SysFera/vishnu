/**
 * \file list_users.cpp
 * This file defines the VISHNU list user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for ostream, cout, endl
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "ListUsers.hpp"                // for ListUsers
#include "ListUsersOptions.hpp"         // for ListUsersOptions
#include "Options.hpp"                  // for Options, ::CONFIG, ::ENV
#include "api_ums.hpp"                  // for listUsers
#include "ecorecpp/mapping/EList.hpp"   // for EList
#include "utils.hpp"                    // for operator<<

using namespace std;
using namespace vishnu;

struct ListUserFunc {

  UMS_Data::ListUsers mlsUsers;
  UMS_Data::ListUsersOptions mlistOptions;
   bool mfull;

  ListUserFunc(UMS_Data::ListUsers lsUsers,UMS_Data::ListUsersOptions listOptions, bool full):
    mlsUsers(lsUsers), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {

    int res = listUsers(sessionKey, mlsUsers, mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlsUsers << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsUsers.getUsers().size(); i++) {
        cout << mlsUsers.getUsers().get(i) << endl;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){

  /******* Parsed value containers ****************/
  string configFile;

  /********** EMF data ************/

  UMS_Data::ListUsers lsUsers;
  UMS_Data::ListUsersOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListUsersOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fAuthSystemId( boost::bind(&UMS_Data::ListUsersOptions::setAuthSystemId,boost::ref(listOptions),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);

  opt->add("userId,u",
      "  allows an admin to get information about \n"
      " a specific user identified by his/her userId",
      CONFIG,
      fUserId);

  opt->add("authSystemId,i",
      "is an option to list users who have local\n"
      "user-authentication configurations on a specific \n"
      "user-authentication system",
      CONFIG,
      fAuthSystemId);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  /********  Process **************************/

  bool full = false;
  // Display the list
  if(isEmpty) {
    full = true;
  }

  ListUserFunc listFunc(lsUsers, listOptions, full);
  return GenericCli().run(listFunc, configFile, ac, av);

}// end of main
