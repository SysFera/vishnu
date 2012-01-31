/**
 * \file list_users.cpp
 * This file defines the VISHNU list user command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include "ListUsersOptions.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

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
  string dietConfig;

  /********** EMF data ************/

  UMS_Data::ListUsers lsUsers;
  UMS_Data::ListUsersOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListUsersOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,string> fAuthSystemId( boost::bind(&UMS_Data::ListUsersOptions::setAuthSystemId,boost::ref(listOptions),_1));

 
  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

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
  return GenericCli().run(listFunc, dietConfig, ac, av);

}// end of main


