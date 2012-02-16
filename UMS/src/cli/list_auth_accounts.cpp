/**
 * \file list_authentication_accounts.cpp
 * This file defines the VISHNU list authentication accounts command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "utilVishnu.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListAuthenticationAccountsFunc {

  UMS_Data::ListAuthAccounts mlsAuthAccounts;
  UMS_Data::ListAuthAccOptions mlistOptions;
  bool mfull;

  ListAuthenticationAccountsFunc(UMS_Data::ListAuthAccounts lsAuthAccounts, UMS_Data::ListAuthAccOptions listOptions, bool full):
    mlsAuthAccounts(lsAuthAccounts), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res =  listAuthAccounts(sessionKey,mlsAuthAccounts,mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlsAuthAccounts << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsAuthAccounts.getAuthAccounts().size(); i++) {
        cout << mlsAuthAccounts.getAuthAccounts().get(i) ;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::ListAuthAccounts lsAuthAccounts;

  UMS_Data::ListAuthAccOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListAuthAccOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fAuthSystemId( boost::bind(&UMS_Data::ListAuthAccOptions::setAuthSystemId,boost::ref(listOptions),_1));


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));


  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);


  opt->add("userId,u",
      "is an admin option for listing all local user-authentication "
      "configurations of a specific user identified by his/her userId",
      CONFIG,
      fUserId);

  opt->add("authSystemId,i",
      "is an option for listing local user-configuration of"
       "a specific user-authentication system",
      CONFIG,
      fAuthSystemId);

  opt->add("listAll,a",
      "is an admin option for listing all local"
      "user-authentication configurations of VISHNU",
      CONFIG
      );

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  if (opt->count("listAll")){
    listOptions.setListAll(true);
  }

  bool full = false;
  // Display the list
  if(isEmpty|| (opt->count("listAll"))) {
    full = true;
  }

  ListAuthenticationAccountsFunc listAuthenticationAccountsFunc (lsAuthAccounts, listOptions, full);
  return GenericCli().run(listAuthenticationAccountsFunc, dietConfig, ac, av);
}// end of main


