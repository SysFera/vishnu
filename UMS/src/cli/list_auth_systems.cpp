/**
 * \file list_authentication_systems.cpp
 * This file defines the VISHNU list authentication systems command
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

struct ListAuthenticationsystemsFunc {

  UMS_Data::ListAuthSystems mlsAuthSystems;
  UMS_Data::ListAuthSysOptions mlistOptions;
  bool mfull;

  ListAuthenticationsystemsFunc(UMS_Data::ListAuthSystems lsAuthSystems, UMS_Data::ListAuthSysOptions listOptions, bool full):
    mlsAuthSystems(lsAuthSystems), mlistOptions(listOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res =  listAuthSystems(sessionKey,mlsAuthSystems,mlistOptions);
    // Display the list
    if(mfull) {
      cout << mlsAuthSystems << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsAuthSystems.getAuthSystems().size(); i++) {
        cout << mlsAuthSystems.getAuthSystems().get(i) ;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::ListAuthSystems lsAuthSystems;

  UMS_Data::ListAuthSysOptions listOptions;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListAuthSysOptions::setUserId,boost::ref(listOptions),_1));

  boost::function1<void,string> fAuthSystemId( boost::bind(&UMS_Data::ListAuthSysOptions::setAuthSystemId,boost::ref(listOptions),_1));


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(new Options(av[0]));


  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("listAllAuthSystems,a",
      "is an option for listing all VISHNU user-authentication systems",
      CONFIG);

  opt->add("listFullInfo,f",
      "s an admin option for listing full VISHNU"
      "user-authentication systems information such as"
      "all concerned only the administrator:"
      "authLogin, authPassword and userPasswordEncryption",
      CONFIG);

  opt->add("userId,u",
      "is an admin option for listing all"
      "user-authentication systems in which a specific"
      "user has local user-authentication configs",
      CONFIG,
      fUserId);

  opt->add("authSystemId,i",
      "is an option for listing a specific user-authentication system",
      CONFIG,
      fAuthSystemId);


  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  if (opt->count("listAllAuthSystems")){
    listOptions.setListAllAuthSystems(true);
  }

  if (opt->count("listFullInfo")){
    listOptions.setListFullInfo(true);
  }


  bool full = false;
  // Display the list
  if(isEmpty|| (opt->count("listAllAuthSystems"))) {
    full = true;
  }

  ListAuthenticationsystemsFunc listAuthenticationsystemsFunc (lsAuthSystems, listOptions, full);
  return GenericCli().run(listAuthenticationsystemsFunc, dietConfig, ac, av);
}// end of main


