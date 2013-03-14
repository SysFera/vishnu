/**
 * \file list_authentication_accounts.cpp
 * This file defines the VISHNU list authentication accounts command
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
#include "ListAuthAccOptions.hpp"       // for ListAuthAccOptions
#include "ListAuthAccounts.hpp"         // for ListAuthAccounts
#include "Options.hpp"                  // for Options, ::CONFIG, ::ENV
#include "api_ums.hpp"                  // for listAuthAccounts
#include "ecorecpp/mapping/EList.hpp"   // for EList
#include "utils.hpp"                    // for operator<<

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

  string configFile;

  /********** EMF data ************/

  UMS_Data::ListAuthAccounts lsAuthAccounts;

  UMS_Data::ListAuthAccOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListAuthAccOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fAuthSystemId( boost::bind(&UMS_Data::ListAuthAccOptions::setAuthSystemId,boost::ref(listOptions),_1));


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));


  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);


  opt->add("userId,u",
      "is an admin option for listing all user-authentication "
      "accounts of a specific user identified by his/her userId",
      CONFIG,
      fUserId);

  opt->add("authSystemId,i",
      "is an option for listing user-authentication account of "
       "a specific user-authentication system",
      CONFIG,
      fAuthSystemId);

  opt->add("listAll,a",
      "is an admin option for listing all"
      " user-authentication accounts of VISHNU",
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
  return GenericCli().run(listAuthenticationAccountsFunc, configFile, ac, av);
}// end of main
