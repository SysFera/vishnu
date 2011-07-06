/**
 * \file list_local_accounts.cpp
 * This file defines the VISHNU list local account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "listMachineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "utilVishnu.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListLocalAccountFunc {

  UMS_Data::ListLocalAccounts mlsLocalAccount;
  UMS_Data::ListLocalAccOptions mlistOptions;
  bool mfull;

  ListLocalAccountFunc(UMS_Data::ListLocalAccounts lsLocalAccount, UMS_Data::ListLocalAccOptions listOptions, bool full):
    mlsLocalAccount(lsLocalAccount), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res =  listLocalAccounts(sessionKey,mlsLocalAccount,mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlsLocalAccount << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsLocalAccount.getAccounts().size(); i++) {
        cout << mlsLocalAccount.getAccounts().get(i) ;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::ListLocalAccounts lsLocalAccount;

  UMS_Data::ListLocalAccOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListLocalAccOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListLocalAccOptions::setMachineId,boost::ref(listOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],fUserId, dietConfig, fMachineId);


  opt->add("adminListOption,a",
           "is an admin option for listing all local configurations of all users	",
           CONFIG);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  if (opt->count("adminListOption")){
    listOptions.setAdminListOption(true);
  }

  bool full = false;
  // Display the list
  if(isEmpty|| (opt->count("adminListOption"))) {
    full = true;
  }
  
  ListLocalAccountFunc listAccountFunc(lsLocalAccount, listOptions, full);
  return GenericCli().run(listAccountFunc, dietConfig, ac, av);
}// end of main


