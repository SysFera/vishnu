#include <boost/test/unit_test.hpp>

// EMF INCLUDE
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
//#include "UMS_Data/UMS_DataPackageImpl.cpp"
#include <iostream>


BOOST_AUTO_TEST_SUITE( test_suite )
using namespace std;
using namespace UMS_Data;


int connect           (string uid, string           pwd , string&                key   , ConnectOptions_ptr op ){key = "lapin" ; return 0;}
int reconnect         (string uid, string           pwd , string                 sid   , string&            key){key = "lapin2"; return 0;}
int listSession       (string key, ListSessions_ptr li  , ListSessionOptions_ptr op    )                      	{    	       	 return 0;}  
int changePassword    (string uid, string           pwd , string                 newPwd)                      	{    	       	 return 0;}
int deleteLocalAccount(string key, string           uid , string                 mid   )                      	{    	       	 return 0;}
int resetPassword     (string uid, string           pwd )                                                     	{    	       	 return 0;}
int updateUser        (string key, User_ptr         user)						      	{    	       	 return 0;}
int deleteUser        (string key, string           user)						      	{    	       	 return 0;}
int addVishnuUser     (string key, User_ptr         user)						      	{    	       	 return 0;}
int deleteMachine     (string key, string           ma  )                                                     	{    	       	 return 0;}
int resetPassword     (string key, int              uid )                                                     	{    	       	 return 0;}
int addLocalAccount   (string key, LocalAccount_ptr acc )                                                     	{    	       	 return 0;}
int addMachine        (string key, Machine_ptr      ma  )                                                     	{    	       	 return 0;}
int updateMachine     (string key, Machine_ptr      ma  )                                                     	{    	       	 return 0;}
int updateLocalAccount(string key, LocalAccount_ptr acc )                                                     	{    	       	 return 0;}
int restore           (string fil)                                                                            	{    	       	 return 0;}
int close             (string key)                                                                            	{    	       	 return 0;}      

BOOST_AUTO_TEST_CASE( my_test )
{
  // CREATE DATA MODEL                                                                                                                                                
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();

  ////
  // Parameters
  ////

  // connect
  int                	 idl  = 4        ;
  string             	 key  = ""       ;
  string 	     	 pwd  = "admin"  ;
  string 	     	 uid  = "admin_1";
  string             	 sid  = "1"      ;
  string             	 sub  = ""       ;
  SessionCloseType   	 sct  = 1        ;  
  ConnectOptions_ptr 	 cop  = ecoreFactory->createConnectOptions();
  ListSessions_ptr   	 li   = ecoreFactory->createListSessions();
  ListSessionOptions_ptr opt  = ecoreFactory->createListSessionOptions();
  // connect as
  string const&      	 subs = "user_1";
  // user
  User_ptr           	 use  = ecoreFactory->createUser();
  PrivilegeType      	 pri  = 1         ;
  string             	 rec  = string("");
  string             	 cu   = "toto_1"  ;
  string             	 pass = "user_1"  ; 
  string             	 fina = "toto"    ;
  string             	 lana = "dupont"  ; 
  string             	 mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  // local account
  LocalAccount_ptr       lacc = ecoreFactory->createLocalAccount();
  string                 mid  = "machine_1" ;
  string                 accL = "toto"      ;
  string                 ssh  = "/usr/local";
  string                 home = "/home/toto";
  // machine
  Machine_ptr            ma   = ecoreFactory->createMachine();
  string                 maid = "machine_2";
  string                 mana = "ulysse"   ;
  string                 site = "paris"    ;
  string                 desc = "calcul"   ;
  string                 lang = "fr"       ;
  // Setting value
  cop->setClosePolicy(sct);
  cop->setSessionInactivityDelay(idl);
  cop->setSubstituteUserId(sub);

  ///////////
  // TESTS //
  ///////////

  // Connect normal call
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  cout << " Testing normal connection U1-B1" << endl;
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  BOOST_CHECK  (listSession(key, li , opt      )==0);
  cout << "Key generated : " << key << endl;

  // Connect with bad uid
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  cout << " Testing with bad uid U1-E1" << endl;
  BOOST_CHECK  (connect    ("bad", pwd, key, cop)!=0);
  BOOST_CHECK  (listSession(key  , li , opt     )!=0);

  // Connect with bad pwd
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  cout << " Testing bad pwd U1-E2"<< endl;
  BOOST_CHECK  (connect    (uid, "bad", key, cop)!=0);
  BOOST_CHECK  (listSession(key, li   , opt     )!=0);


  // Connect with timeout
  sct = 0;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  cout << " Testing timeout U1.1-B1" << endl;
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  sleep(10);
  BOOST_CHECK  (listSession(key, li , opt      )!=0);

  // Connect with bad uid
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  cout << " Testing with bad uid U1.1-E1" << endl;
  BOOST_CHECK  (connect    ("bad", pwd, key, cop)!=0);
  BOOST_CHECK  (listSession(key  , li , opt     )!=0);

  // Connect with bad pwd
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  cout << " Testing bad pwd U1.1-E2"<< endl;
  BOOST_CHECK  (connect    (uid, "bad", key, cop)!=0);
  BOOST_CHECK  (listSession(key, li   , opt     )!=0);

  // Connect with bad closure policy
  sct = 3;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing unknown closure mode U1.1-E3"<< endl;
  BOOST_CHECK  (connect(uid, pwd, key, cop )!=0);
  sct = 0;
  cop->setClosePolicy(sct);

  // Connect with a temporary password
  BOOST_REQUIRE(restore    ("clean_session_tempPwd.sql")==0);
  cout << " Testing temporary pwd U1.1-E4"<< endl;
  BOOST_CHECK  (connect    (uid, pwd, key, cop         )!=0);
  BOOST_CHECK  (listSession(key, li , opt              )==0);

  // Connect as an other user
  cop->setSubstituteUserId(subs);
  BOOST_REQUIRE(restore    ("clean_session_tempPwd.sql")==0);
  cout << " Testing another user session U1.1-E5"<< endl;
  BOOST_CHECK  (connect    (uid, pwd, key, cop         )!=0);
  BOOST_CHECK  (listSession(key, li , opt              )!=0);
  cop = ecoreFactory->createConnectOptions();

  sct = 0;
  cop->setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  cout << " Testing connect on disconnect U1.1-B2" << endl;
  BOOST_CHECK  (connect(uid, pwd, key, cop )==0);
  // Fork to kill and delete the proc and test the closure
  switch (fork()){

  case 0 : // child
    sleep(5); // Sleep to make sure father has returned (->session closed)
    BOOST_CHECK(listSession(key, li, opt)!=0);
    
    // ReConnect normal call
    // -> connect
    BOOST_REQUIRE(restore    ("clean_session.sql")==0);
    cout << " Testing normal connection U1-B1" << endl;
    BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
    BOOST_CHECK  (listSession(key, li , opt      )==0);
    // -> and then reconnect normal
    if (li->getSessions().size() &&
	(li->getSessions())[0])
      rec = li->getSessions()[0]->getSessionId();
    BOOST_CHECK(reconnect  (uid, pwd, rec, key)==0);
    BOOST_CHECK(listSession(key, li , opt     )==0);


    // Reconnect with bad user id
    BOOST_REQUIRE(restore  ("clean_session.sql" )==0);
    cout << " Testing error uid reconnect U1.5-E1"<< endl;
    BOOST_CHECK  (reconnect("bad", pwd, sid, key)!=0);

    // Reconnect with bad password
    BOOST_REQUIRE(restore  ("clean_session.sql" )==0);
    cout << " Testing error pwd reconnect U1.5-E2"<< endl;
    BOOST_CHECK  (reconnect(uid, "bad", sid, key)!=0);

    // Test normal close
    BOOST_REQUIRE(restore    ("clean_session.sql")==0);
    cout << " Testing normal close U1.2B"<< endl;
    BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
    BOOST_CHECK  (listSession(key, li , opt      )==0);
    BOOST_CHECK  (close      (key                )==0);
    BOOST_CHECK  (listSession(key, li, opt       )!=0);

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
    use->setUserId   (cu)  ;
    use->setPassword (pass);
    use->setFirstname(fina);
    use->setLastname (lana);
    use->setPrivilege(pri) ;
    use->setEmail    (mail);
    BOOST_CHECK(addVishnuUser(key, use)==0);
    BOOST_CHECK(close        (key     )==0);

    // Login already in db
    cout << " Testing adding the same user twice U4-E"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addVishnuUser(key, use           )==0);
    BOOST_CHECK	 (addVishnuUser(key, use           )!=0);
    BOOST_CHECK	 (close        (key                )==0);

    // Add user bad email
    cout << " Testing adding a user with a bad email U4-E"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
    use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
    BOOST_CHECK  (addVishnuUser(key, use           )!=0);
    BOOST_CHECK  (close        (key                )==0);
    use->setEmail(mail);

    // Update valid
    cout << " Testing valid update user U4.1-B"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK  (addVishnuUser(key, use           )==0);
    use->setEmail("cl3m3ntlebgkidechyrGRAVE@hotmail.fr");
    BOOST_CHECK  (updateUser   (key, use           )==0);
    use->setEmail(mail);
    BOOST_CHECK  (close        (key                )==0);

    // Login invalid to update
    cout << " Testing bad update parameter bad user id U4.1-E"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql"     )==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop)==0);
    use->setUserId("");
    BOOST_CHECK	 (addVishnuUser(key, use	  )==0);    
    BOOST_CHECK	 (updateUser   (key, use	  )!=0);
    BOOST_CHECK	 (close        (key               )==0);
    use->setUserId(cu);

    // Update user fails : bad email
    cout << " Testing update a user with a bad email U4.1-E"    <<endl;
    BOOST_REQUIRE(restore   ("clean_session.sql")==0);
    BOOST_CHECK  (connect   (uid, pwd, key, cop )==0);
    use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
    BOOST_CHECK  (updateUser(key, use           )!=0);
    BOOST_CHECK  (close     (key                )==0);
    use->setEmail(mail);

    // Delete user 
    cout << " Testing delete normal U4.2B"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addVishnuUser(key, use	   )==0);     
    BOOST_CHECK	 (deleteUser   (key, cu 	   )==0);
    BOOST_CHECK	 (close        (key                )==0);

    // Delete user  bad uid
    cout << " Testing delete bad uid U4.2E"    <<endl;
    BOOST_REQUIRE(restore   ("clean_session.sql")==0);
    BOOST_CHECK	 (connect   (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (deleteUser(key, cu            )!=0);
    BOOST_CHECK	 (close     (key                )==0);


    // Change pwd ok
    cout << " Testing change password normal U1.3.3"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql"       )==0);
    BOOST_CHECK	 (connect       (uid, pwd , key, cop)==0);
    BOOST_CHECK	 (addVishnuUser (key, use           )==0);
    BOOST_CHECK	 (changePassword(cu , pass, "newPwd")==0);
    BOOST_CHECK	 (close         (key                )==0);

    // Change pwd bad uid
    cout << " Testing change password bad uid U1.3.3E"    <<endl;
    BOOST_REQUIRE(restore	("clean_session.sql"  )==0);
    BOOST_CHECK	 (connect	(uid  , pwd, key, cop )==0);
    BOOST_CHECK	 (addVishnuUser (key  , use           )==0);
    BOOST_CHECK	 (changePassword("bad", pass, "newPwd")!=0);
    BOOST_CHECK	 (close         (key                  )==0);

    // Change pwd bad pwd
    cout << " Testing change password bad pwd U1.3.3E"    <<endl;
    BOOST_REQUIRE(restore	("clean_session.sql")==0);
    BOOST_CHECK	 (connect	(uid, pwd , key, cop)==0);
    BOOST_CHECK	 (addVishnuUser (key, use           )==0);
    BOOST_CHECK	 (changePassword(cu, "bad", "newPwd")!=0);
    BOOST_CHECK	 (close         (key                )==0);

    // Reset pwd ok
    cout << " Testing reset password normal UA2-B"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addVishnuUser(key, use           )==0);
    BOOST_CHECK	 (resetPassword(key, cu            )==0);
    BOOST_CHECK	 (close        (key                )==0);

    // Reset pwd bad uid
    cout << " Testing reset password bad uid UA2-E"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql" )==0);
    BOOST_CHECK	 (connect      (uid, pwd  , key, cop)==0);
    BOOST_CHECK	 (addVishnuUser(key, use            )==0);
    BOOST_CHECK	 (resetPassword(key, "bad"          )!=0);
    BOOST_CHECK	 (close        (key                 )==0);

    // Add local account
    cout << " Testing add local account success U4-B"    <<endl;
    BOOST_REQUIRE(restore("clean_session.sql")==0);
    BOOST_CHECK  (connect(uid, pwd, key, cop )==0);
    lacc->setUserId       (uid);
    lacc->setMachineId    (mid);
    lacc->setAcLogin     (accL);
    lacc->setSshKeyPath   (ssh);
    lacc->setHomeDirectory(home);
    BOOST_CHECK(addLocalAccount(key, lacc)==0);
    BOOST_CHECK(close          (key      )==0);

    // Add local account bad machine id
    cout << " Testing add local account bad machine U4-E"    <<endl;
    BOOST_REQUIRE(restore        ("clean_session.sql")==0);
    BOOST_CHECK  (connect        (uid, pwd , key, cop)==0);
    lacc->setMachineId("bad");
    BOOST_CHECK	 (addLocalAccount(key, lacc  	     )!=0);
    BOOST_CHECK	 (close          (key        	     )==0);  
    lacc->setMachineId(mid);

    // Add local account bad acc login
    cout << " Testing add local account success U4-E"    <<endl;
    BOOST_REQUIRE(restore	 ("clean_session.sql")==0);
    BOOST_CHECK  (connect	 (uid, pwd, key, cop )==0);
    lacc->setAcLogin("bad");
    BOOST_CHECK	 (addLocalAccount(key, lacc	     )!=0);
    BOOST_CHECK	 (close          (key      	     )==0);    
    lacc->setAcLogin(accL);

    // Update local account
    cout << " Testing update local account success U4.1-B"    <<endl;
    BOOST_REQUIRE(restore	  ("clean_session.sql")==0);
    BOOST_CHECK  (connect	  (uid, pwd , key, cop)==0);
    lacc->setSshKeyPath("/usr/bin");
    BOOST_CHECK(addLocalAccount   (key, lacc	      )==0);    
    BOOST_CHECK(updateLocalAccount(key, lacc	      )==0);
    BOOST_CHECK(close             (key      	      )==0);
    lacc->setSshKeyPath(ssh);

    // Update local account bad machine id
    cout << " Testing update local account bad machine U4.1-E"    <<endl;
    BOOST_REQUIRE(restore           ("clean_session.sql")==0);
    BOOST_CHECK  (connect           (uid, pwd , key, cop)==0);
    BOOST_CHECK	 (addLocalAccount   (key, lacc  	)!=0);
    lacc->setMachineId("bad");
    BOOST_CHECK  (updateLocalAccount(key, lacc	        )==0);
    BOOST_CHECK	 (close             (key        	)==0);  
    lacc->setMachineId(mid);

    // Delete local account 
    cout << " Testing delete local account normal U4.2B"    <<endl;
    BOOST_REQUIRE(restore           ("clean_session.sql")==0);
    BOOST_CHECK  (connect           (uid, pwd , key, cop)==0);
    BOOST_CHECK	 (addLocalAccount   (key, lacc          )==0);
    BOOST_CHECK	 (deleteLocalAccount(key, uid , mid     )==0);
    BOOST_CHECK	 (close             (key                )==0);  

    // Delete local accountbad uid
    cout << " Testing delete local account bad uid U4.2E"    <<endl;
    BOOST_REQUIRE(restore           ("clean_session.sql" )==0);
    BOOST_CHECK  (connect           (uid, pwd  , key, cop)==0);
    BOOST_CHECK	 (addLocalAccount   (key, lacc           )==0);
    BOOST_CHECK	 (deleteLocalAccount(key, "bad", mid     )!=0);
    BOOST_CHECK	 (close             (key                 )==0);  

    // Delete local accountbad uid
    cout << " Testing delete local account bad machine"    <<endl;
    BOOST_REQUIRE(restore           ("clean_session.sql"  )==0);
    BOOST_CHECK  (connect           (uid, pwd , key  , cop)==0);
    BOOST_CHECK	 (addLocalAccount   (key, lacc            )==0);
    BOOST_CHECK	 (deleteLocalAccount(key, uid , "bad"     )!=0);
    BOOST_CHECK	 (close             (key                  )==0);  

    // Test add machine normal
    cout << " Testing add machine UA6.1B"    <<endl;
    BOOST_REQUIRE(restore    ("clean_session.sql")==0);
    BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
    ma->setMachineId         (maid);
    ma->setName              (mana);
    ma->setSite              (site);
    ma->setMachineDescription(desc);
    ma->setLanguage          (lang);
    BOOST_CHECK	 (addMachine (key, ma            )==0);
    BOOST_CHECK	 (close      (key                )==0);  

    // Machine already exist
    cout << " Testing add machine twice the same UA6.1E"    <<endl;
    BOOST_REQUIRE(restore    ("clean_session.sql")==0);
    BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addMachine (key, ma            )==0);
    BOOST_CHECK	 (addMachine (key, ma            )!=0);
    BOOST_CHECK	 (close      (key                )==0);  

    // Test update machine normal
    cout << " Testing update machine UA6.4B"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addMachine   (key, ma            )==0);
    ma->setName("Machina");
    BOOST_CHECK	 (updateMachine(key, ma            )==0);
    BOOST_CHECK	 (close        (key                )==0);  
    ma->setName(mana);

    // Test update machine normal
    cout << " Testing update machine bad machien id UA6.4E"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addMachine   (key, ma            )==0);
    ma->setMachineId("bad");
    BOOST_CHECK	 (updateMachine(key, ma            )!=0);
    BOOST_CHECK	 (close        (key                )==0);  
    ma->setMachineId(maid);

    // Test delete machine normal
    cout << " Testing update machine UA6.2B"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd , key, cop)==0);
    BOOST_CHECK	 (addMachine   (key, ma            )==0);
    BOOST_CHECK	 (deleteMachine(key, maid          )==0);
    BOOST_CHECK	 (close        (key                )==0);  

    // Test delete machine normal
    cout << " Testing update machine bad machien id UA6.2E"    <<endl;
    BOOST_REQUIRE(restore      ("clean_session.sql")==0);
    BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
    BOOST_CHECK	 (addMachine   (key, ma            )==0);
    ma->setMachineId("bad");
    BOOST_CHECK	 (updateMachine(key, ma            )!=0);
    BOOST_CHECK	 (close        (key                )==0);  
    ma->setMachineId(maid);


    break;
  case -1 :
    cerr << "Error forking "<< endl;
    break;
  default :
    return;
  }


}

BOOST_AUTO_TEST_SUITE_END()
