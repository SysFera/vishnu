#include "reconnect.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string userId;

	string password;

	string sessionId;

	string dietConfig;

/********* Out parameters     *****************/

  UMS_Data::Session session;

  /**************** Describe options *************/


  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1, dietConfig);

  opt->setPosition("userId",1);


  opt->add("sessionId,s",
           "represents the identifier of the session",
           HIDDEN,
           sessionId,1);

  opt->setPosition("sessionId",1);



  try {

    /**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();


/********  Process **************************/

		checkVishnuConfig(*opt);


    //Fix me
    password=takePassword("Password: ");

    cout << "The user password is " << password << endl;


    /************** Call UMS reconnect service *******************************/

    
    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

    cerr << "DIET initialization failed !" << endl;

    return 1;
    }


//    reconnect(userId, password, sessionId, session);
   

    // store the session information

    storeLastSessionKey(session.getSessionKey(),session.getClosePolicy(),getppid());


  }// End of try bloc

  catch(po::required_option& e){

    usage(*opt,"[options] userId sessionId","required parameter is missing");

  }
  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){
    errorUsage(av[0], e.what()) ;
    return 1;
  }

  return 0;

}// end of main


