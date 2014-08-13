/**
 * \file list_local_accounts.cpp
 * This file defines the VISHNU list local account command
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
#include "ListLocalAccOptions.hpp"      // for ListLocalAccOptions
#include "ListLocalAccounts.hpp"        // for ListLocalAccounts
#include "Options.hpp"                  // for Options, ::CONFIG
#include "api_ums.hpp"                  // for listLocalAccounts
#include "ecorecpp/mapping/EList.hpp"   // for EList
#include "listMachineUtils.hpp"         // for makeListMachineOptions
#include "utils.hpp"                    // for operator<<

struct ListLocalAccountFunc {

  UMS_Data::ListLocalAccounts mlsLocalAccount;
  UMS_Data::ListLocalAccOptions mlistOptions;
  bool mfull;

  ListLocalAccountFunc(UMS_Data::ListLocalAccounts lsLocalAccount, UMS_Data::ListLocalAccOptions listOptions, bool full):
    mlsLocalAccount(lsLocalAccount), mlistOptions(listOptions), mfull(full)
  {}

  int operator()(std::string sessionKey) {
    int res =  vishnu::listLocalAccounts(sessionKey,mlsLocalAccount,mlistOptions);
    // Display the list
    if(mfull) {
      std::cout << mlsLocalAccount << std::endl;
    }
    else {
      for(unsigned int i = 0; i < mlsLocalAccount.getAccounts().size(); i++) {
        std::cout << mlsLocalAccount.getAccounts().get(i) ;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  std::string configFile;

  /********** EMF data ************/

  UMS_Data::ListLocalAccounts lsLocalAccount;

  UMS_Data::ListLocalAccOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,std::string> fUserId( boost::bind(&UMS_Data::ListLocalAccOptions::setUserId,boost::ref(listOptions),_1));
  boost::function1<void,std::string> fMachineId( boost::bind(&UMS_Data::ListLocalAccOptions::setMachineId,boost::ref(listOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],
      fUserId,
      configFile,
      fMachineId);

  opt->add("adminListOption,a",
           "is an admin option for listing all local configurations of all users",
           CONFIG);
  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);
  if (opt->count("adminListOption")){
    listOptions.setAdminListOption(true);
  }

  bool full = false;
  if(isEmpty || (opt->count("adminListOption"))) {
    full = true;
  }
  ListLocalAccountFunc listAccountFunc(lsLocalAccount, listOptions, full);
  return GenericCli().run(listAccountFunc, configFile, ac, av);
}// end of main
