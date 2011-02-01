#include <boost/test/unit_test.hpp>

// EMF INCLUDE
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
//#include "UMS_Data/UMS_DataPackageImpl.cpp"
#include <iostream>


BOOST_AUTO_TEST_SUITE( test_suite )
using namespace std;
using namespace UMS_Data;


int connect(string uid, string pwd, string& key, ConnectOptions_ptr op){key="lapin"; return 0;}
int close(string key){return 0;}
int reconnect(string uid, string pwd, string sid, string& key){key = "lapin2"; return 0;}
int listSession(string key, ListSessions_ptr li, ListSessionOptions_ptr op){return 0;};
int restore(string file){return 0;}
int updateUser(string key, User_ptr user){return 0;}
int deleteUser(string key, string user){return 0;}
int addVishnuUser(string key, User_ptr user){return 0;}

BOOST_AUTO_TEST_CASE( my_test )
{
  // CREATE DATA MODEL                                                                                                                                                
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();

  ////
  // Parameters
  ////

  // connect
  int                	 idl  = 4;
  string             	 key  = "";
  string 	     	 pwd  = "admin";
  string 	     	 uid  = "admin_1";
  string             	 sid  = "1";
  string             	 sub  = "";
  SessionCloseType   	 sct  = 1; 
  ConnectOptions_ptr 	 cop  = ecoreFactory->createConnectOptions();
  ListSessions_ptr   	 li   = ecoreFactory->createListSessions();
  ListSessionOptions_ptr opt  = ecoreFactory->createListSessionOptions();
  // connect as
  string const&      	 subs = "user_1";
  // user
  User_ptr           	 use  = ecoreFactory->createUser();
  PrivilegeType      	 pri  = 1;
  string             	 rec  = string("");
  string             	 cu   = "toto_1";
  string             	 pass = "user_1"; 
  string             	 fina = "toto";
  string             	 lana = "dupont";
  string             	 mail = "cl3m3ntlebgkidechyr@hotmail.fr";

  // Setting value
  cop->setClosePolicy(sct);
  cop->setSessionInactivityDelay(idl);
  cop->setSubstituteUserId(sub);



  ///////////
  // TESTS //
  ///////////

  // Connect normal call
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing normal connection U1-B1" << endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)==0);
  BOOST_CHECK(listSession(key, li, opt)==0);
  cout << "Key generated : " << key << endl;

  // Connect with bad uid
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing with bad uid U1-E1" << endl;
  BOOST_CHECK(connect("bad", pwd, key, cop)!=0);
  BOOST_CHECK(listSession(key, li, opt)!=0);

  // Connect with bad pwd
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing bad pwd U1-E2"<< endl;
  BOOST_CHECK(connect(uid, "bad", key, cop)!=0);
  BOOST_CHECK(listSession(key, li, opt)!=0);


  // Connect with timeout
  sct = 0;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing timeout U1.1-B1" << endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)==0);
  sleep(10);
  BOOST_CHECK(listSession(key, li, opt)!=0);

  // Connect with bad uid
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing with bad uid U1.1-E1" << endl;
  BOOST_CHECK(connect("bad", pwd, key, cop)!=0);
  BOOST_CHECK(listSession(key, li, opt)!=0);

  // Connect with bad pwd
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing bad pwd U1.1-E2"<< endl;
  BOOST_CHECK(connect(uid, "bad", key, cop)!=0);
  BOOST_CHECK(listSession(key, li, opt)!=0);

  // Connect with bad closure policy
  sct = 3;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing unknown closure mode U1.1-E3"<< endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)!=0);
  sct = 0;
  cop->setClosePolicy(sct);

  // Connect with a temporary password
  BOOST_REQUIRE(restore("clean_session_tempPwd.sql")==0);
  cout << " Testing temporary pwd U1.1-E4"<< endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)!=0);
  BOOST_CHECK(listSession(key, li, opt)==0);

  // Connect as an other user
  cop->setSubstituteUserId(subs);
  BOOST_REQUIRE(restore("clean_session_tempPwd.sql")==0);
  cout << " Testing another user session U1.1-E5"<< endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)!=0);
  cop = ecoreFactory->createConnectOptions();
  BOOST_CHECK(listSession(key, li, opt)!=0);

  sct = 0;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing connect on disconnect U1.1-B2" << endl;
  BOOST_CHECK(connect(uid, pwd, key, cop)==0);
  // Fork to kill and delete the proc and test the closure
  switch (fork()){

  case 0 : // child
    sleep(5); // Sleep to make sure father has returned (->session closed)
    BOOST_CHECK(listSession(key, li, opt)!=0);
    
    // ReConnect normal call
    // -> connect
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    cout << " Testing normal connection U1-B1" << endl;
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(listSession(key, li, opt)==0);
    // -> and then reconnect normal
    if (li->getSessions().size() &&
	(li->getSessions())[0])
      rec = li->getSessions()[0]->getSessionId();
    BOOST_CHECK(reconnect(uid, pwd, rec, key)==0);
    BOOST_CHECK(listSession(key, li, opt)==0);


    // Reconnect with bad user id
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    cout << " Testing error uid reconnect U1.5-E1"<< endl;
    BOOST_CHECK(reconnect("bad", pwd, sid, key)!=0);

    // Reconnect with bad password
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    cout << " Testing error pwd reconnect U1.5-E2"<< endl;
    BOOST_CHECK(reconnect(uid, "bad", sid, key)!=0);

    // Test normal close
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    cout << " Testing normal close U1.2B"<< endl;
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(listSession(key, li, opt)==0);
    BOOST_CHECK(close(key)==0);
    BOOST_CHECK(listSession(key, li, opt)!=0);

    // Test close with bad sid
    cout << " Testing close a bad session U1.2E1"<< endl;
    BOOST_CHECK(close("toto")!=0);

    // Test closing an already closed session
    cout << " Testing close a closed session U1.2E2"<< endl;
    BOOST_CHECK(close(key)!=0);

    // Add user
    cout << " Testing adding a user U4-B"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    use->setUserId(cu);
    use->setPassword(pass);
    use->setFirstname(fina);
    use->setLastname(lana);
    use->setPrivilege(pri);
    use->setEmail(mail);
    BOOST_CHECK(addVishnuUser(key, use)==0);
    BOOST_CHECK(close(key)==0);

    // Login already in db
    cout << " Testing adding the same user twice"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(addVishnuUser(key, use)==0);
    BOOST_CHECK(addVishnuUser(key, use)!=0);
    BOOST_CHECK(close(key)==0);

    // Add user bad email
    cout << " Testing adding a user with a bad email U4-E"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
    BOOST_CHECK(addVishnuUser(key, use)!=0);
    BOOST_CHECK(close(key)==0);
    use->setEmail(mail);

    // Update valid
    cout << " Testing valid update user"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(addVishnuUser(key, use)==0);
    use->setEmail("cl3m3ntlebgkidechyrGRAVE@hotmail.fr");
    BOOST_CHECK(updateUser(key, use)==0);
    use->setEmail(mail);
    BOOST_CHECK(close(key)==0);

    // Login invalid to update
    cout << " Testing bad update parameter (bad user id)"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    use->setUserId("");
    BOOST_CHECK(addVishnuUser(key, use)==0);
    BOOST_CHECK(updateUser(key, use)!=0);
    BOOST_CHECK(close(key)==0);
    use->setUserId(cu);

    // Update user fails : bad email
    cout << " Testing update a user with a bad email "    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
    BOOST_CHECK(updateUser(key, use)!=0);
    BOOST_CHECK(close(key)==0);
    use->setEmail(mail);

    // Delete user account 
    cout << " Testing delete normal"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(addVishnuUser(key, use)==0);
    BOOST_CHECK(deleteUser(key, cu)==0);
    BOOST_CHECK(close(key)==0);

    // Delete user account bad uid
    cout << " Testing delete bad uid"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK(connect(uid, pwd, key, cop)==0);
    BOOST_CHECK(deleteUser(key, cu)!=0);
    BOOST_CHECK(close(key)==0);

    break;
  case -1 :
    cerr << "Error forking "<< endl;
    break;
  default :
    return;
  }


}

BOOST_AUTO_TEST_SUITE_END()
