#include <boost/test/unit_test.hpp>

// EMF INCLUDE
//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include <iostream>
#include "api_ums.hpp"
#include "config.h"

BOOST_AUTO_TEST_SUITE( test_suite )
using namespace std;
using namespace UMS_Data;



BOOST_AUTO_TEST_CASE( my_test )
{

  int argc = 2;
  char* argv[argc];
  argv[0]= (char*)"./automTest";
  argv[1]=getenv("VISHNU_CONFIG_FILE");
  if (diet_initialize(argv[1], argc, argv)) {
    BOOST_TEST_MESSAGE( "Error in diet_initialize..." );
  }

  // CREATE DATA MODEL                                                                                                                                                
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();

  string sqlScript = TESTCONF;

  ////
  // Parameters
  ////
  int i;
  // connect
  int                	  idl  = 4        ;
  string             	  key  = ""       ;
  string                  key2 = ""       ;
  string 	     	  pwd  = "admin"  ;
  string 	     	  uid  = "admin_1";
  string             	  sid  = "1"      ;
  string             	  sub  = ""       ;
  SessionCloseType   	  sct  = 1        ;  
  ConnectOptions 	  cop  ;//= ecoreFactory->createConnectOptions();
  ListSessions_ptr   	  li   = ecoreFactory->createListSessions();
  ListSessionOptions      opt  ;//= ecoreFactory->createListSessionOptions();
  string 	     	  pwdu = "user"  ;
  string 	     	  uidu = "user_1";
  // connect as
  string const&      	  subs = "user_1";
  // user
  User_ptr           	  use  = ecoreFactory->createUser();
  PrivilegeType      	  pri  = 1         ;
  string             	  rec  = string("");
  string             	  cu   = "toto_1"  ;
  string             	  pass = "user_1"  ; 
  string             	  fina = "toto"    ;
  string             	  lana = "dupont"  ; 
  string             	  mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  ListUsers_ptr      	  liu  = ecoreFactory->createListUsers();
  // local account
  ListLocalAccounts_ptr   lia  = ecoreFactory->createListLocalAccounts();
  ListLocalAccOptions     lioa ;//= ecoreFactory->createListLocalAccOptions();
  LocalAccount            lacc ;//= ecoreFactory->createLocalAccount();
  string                  mid  = "M_1" ;
  string                  accL = "toto"      ;
  string                  ssh  = "/usr/local";
  string                  home = "/home/toto";
  // machine
  ListMachines_ptr        lim  = ecoreFactory->createListMachines();
  ListMachineOptions      liom ;//= ecoreFactory->createListMachineOptions();
  Machine                 ma   ;//= ecoreFactory->createMachine();
  string                  maid = "machine_2";
  string                  mana = "ulysse"   ;
  string                  site = "paris"    ;
  string                  desc = "calcul"   ;
  string                  lang = "fr"       ;
  // Option value
  string                  oval = "CHOC"     ;
  string                  ona  = "NUTE"     ;
  OptionValue             opva ;//= ecoreFactory->createOptionValue();
  ListOptOptions          lioo ;//= ecoreFactory->createListOptOptions();
  ListOptionsValues_ptr   liov = ecoreFactory->createListOptionsValues();
  // Init
  string                  path = "/tmp/vishnu.cfg";
  // Command history
  ListCommands_ptr        lic  = ecoreFactory->createListCommands();
  ListCmdOptions          lico ;//= ecoreFactory->createListCmdOptions();
  // Configuration
  Configuration           conf ;//= ecoreFactory->createConfiguration();
  string                  cpath = "/tmp/confile";

  // Setting value
  cop.setClosePolicy(sct);
  cop.setSessionInactivityDelay(idl);
  cop.setSubstituteUserId(sub);

  ///////////
  // TESTS //
  ///////////
  BOOST_REQUIRE(restore    ("initTest.sql")==0);


  // Connect normal call
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing normal connection U1-B1" );
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  BOOST_CHECK  (listSessions(key, *li , opt      )==0);
  BOOST_MESSAGE("Key generated : " << key );

  // Connect with bad uid
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing with bad uid U1-E1" );
  BOOST_CHECK_THROW  (connect    ("bad", pwd, key, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(key  , *li , opt     ), VishnuException);

  // Connect with bad pwd
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad pwd U1-E2");
  BOOST_CHECK_THROW  (connect    (uid, "bad", key, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(key, *li   , opt     ), VishnuException);


  // Connect with timeout
  sct = 0;
  cop.setClosePolicy(sct);
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing timeout U1.1-B1" );
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  sleep(10);
  BOOST_CHECK_THROW  (listSessions(key, *li , opt      ), VishnuException);

  // Connect with bad uid
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing with bad uid U1.1-E1" );
  BOOST_CHECK_THROW  (connect    ("bad", pwd, key, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(key  , *li , opt     ), VishnuException);

  // Connect with bad pwd
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad pwd U1.1-E2");
  BOOST_CHECK_THROW  (connect    (uid, "bad", key, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(key, *li   , opt     ), VishnuException);

  // Connect with bad closure policy
  sct = 3;
  cop.setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing unknown closure mode U1.1-E3");
  BOOST_CHECK_THROW  (connect(uid, pwd, key, cop ), VishnuException);
  sct = 0;
  cop.setClosePolicy(sct);

  // Connect with a temporary password
  BOOST_REQUIRE(restore    ("clean_session_tempPwd.sql")==0);
  BOOST_MESSAGE(" Testing temporary pwd U1.1-E4");
  BOOST_CHECK_THROW  (connect    (uid, pwd, key, cop         ), VishnuException);
  BOOST_CHECK  (listSessions(key, *li , opt              )==0);

  // Connect as an other user
  cop.setSubstituteUserId(subs);
  BOOST_REQUIRE(restore    ("clean_session_tempPwd.sql")==0);
  BOOST_MESSAGE(" Testing another user session U1.1-E5");
  BOOST_CHECK_THROW  (connect    (uid, pwd, key, cop         ), VishnuException);
  BOOST_CHECK_THROW  (listSessions(key, *li , opt              ), VishnuException);
  cop = *(ecoreFactory->createConnectOptions());

  sct = 0;
  cop.setClosePolicy(sct);
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing connect on disconnect U1.1-B2" );
  BOOST_CHECK  (connect(uid, pwd, key, cop )==0);
  BOOST_CHECK_THROW(listSessions(key, *li, opt), VishnuException);
  
   // ReConnect normal call
   // -> connect
   BOOST_REQUIRE(restore    ("clean_session.sql")==0);
   BOOST_MESSAGE(" Testing normal connection U1-B1" );
   BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);

   li   = ecoreFactory->createListSessions();

   BOOST_CHECK  (listSessions(key, *li , opt      )==0);
   // -> and then reconnect normal
   if (li->getSessions().size() &&
       (li->getSessions())[0]){
     rec = li->getSessions()[0]->getSessionId();
     std::cout << "session_id : "<< rec << std::endl;
     BOOST_CHECK(reconnect  (uid, pwd, rec, key)==0);
     BOOST_CHECK(listSessions(key, *li , opt     )==0);
   }else{
     BOOST_MESSAGE("FAILURE INVALID SESSION KEY TO RECONNECT");
  }

  // Reconnect with bad user id
  BOOST_REQUIRE(restore  ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing error uid reconnect U1.5-E1");
  BOOST_CHECK_THROW  (reconnect("bad", pwd, sid, key), VishnuException);

  // Reconnect with bad password
  BOOST_REQUIRE(restore  ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing error pwd reconnect U1.5-E2");
  BOOST_CHECK_THROW  (reconnect(uid, "bad", sid, key), VishnuException);

  // Test normal close
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing normal close U1.2B");
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  BOOST_CHECK  (listSessions(key, *li , opt      )==0);
  BOOST_CHECK  (close      (key                )==0);
  BOOST_CHECK_THROW  (listSessions(key, *li,  opt      ), VishnuException);

  // Test close with bad sid
  BOOST_MESSAGE(" Testing close a bad session U1.2E1");
  BOOST_CHECK_THROW(close("toto"), VishnuException);

  // Test closing an already closed session
  BOOST_MESSAGE(" Testing close a closed session U1.2E2");
  BOOST_CHECK_THROW(close(key), VishnuException);

  // Add user
  BOOST_MESSAGE(" Testing adding a user U4-B"    );
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  BOOST_CHECK(connect(uid, pwd, key, cop)==0);
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);
  BOOST_CHECK(addUser(key, *use)==0);
  BOOST_CHECK(close        (key     )==0);
  // Login already in db
  BOOST_MESSAGE(" Testing adding the same user twice U4-E"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (addUser(key, *use           )==0);
  BOOST_CHECK_THROW	 (addUser(key, *use           ), VishnuException);
  BOOST_CHECK	 (close        (key                )==0);

  // Add user bad email
  BOOST_MESSAGE(" Testing adding a user with a bad email U4-E"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
  use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
  BOOST_CHECK_THROW  (addUser(key, *use           ), VishnuException);
  BOOST_CHECK  (close        (key                )==0);
  use->setEmail(mail);

   // Update valid
  BOOST_MESSAGE(" Testing valid update user U4.1-B"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
  BOOST_CHECK  (addUser(key, *use           )==0);
  use->setEmail("cl3m3ntlebgkidechyrGRAVE@hotmail.fr");
  BOOST_CHECK (updateUser   (key, *use           )==0);
  use->setEmail(mail);
  BOOST_CHECK  (close        (key                )==0);

  // Login invalid to update
  BOOST_MESSAGE(" Testing bad update parameter bad user id U4.1-E"    );
  BOOST_REQUIRE(restore("clean_session.sql"     )==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop)==0);
  use->setUserId("");
  BOOST_CHECK	 (addUser(key, *use	  )==0);    
  BOOST_CHECK_THROW	 (updateUser   (key, *use	  ), VishnuException);
  BOOST_CHECK	 (close        (key               )==0);
  use->setUserId(cu);

  // Update user fails : bad email
  BOOST_MESSAGE(" Testing update a user with a bad email U4.1-E"    );
  BOOST_REQUIRE(restore   ("clean_session.sql")==0);
  BOOST_CHECK  (connect   (uid, pwd, key, cop )==0);
  use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
  BOOST_CHECK_THROW  (updateUser(key, *use           ), VishnuException);
  BOOST_CHECK  (close     (key                )==0);
  use->setEmail(mail);

  // Delete user 
  BOOST_MESSAGE(" Testing delete normal U4.2B"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
  use  = ecoreFactory->createUser();
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);
  BOOST_CHECK	 (addUser(key, *use	   )==0);     
  BOOST_CHECK	 (deleteUser   (key, use->getUserId() 	   )==0);
  BOOST_CHECK	 (close        (key                )==0);

  // Delete user  bad uid
  BOOST_MESSAGE(" Testing delete bad uid U4.2E"    );
  BOOST_REQUIRE(restore   ("clean_session.sql")==0);
  BOOST_CHECK	 (connect   (uid, pwd, key, cop )==0);
  BOOST_CHECK_THROW	 (deleteUser(key, cu            ), VishnuException);
  BOOST_CHECK	 (close     (key                )==0);


  // Change pwd ok
  BOOST_MESSAGE(" Testing change password normal U1.3.3"    );
  BOOST_REQUIRE(restore("clean_session.sql"       )==0);
  BOOST_CHECK	 (connect       (uid, pwd , key, cop)==0);
  BOOST_CHECK	 (changePassword(uid , pwd, "newPwd")==0);
  BOOST_CHECK	 (changePassword(uid , "newPwd", pwd)==0);
  BOOST_CHECK	 (close         (key                )==0);

  // Change pwd bad uid
  BOOST_MESSAGE(" Testing change password bad uid U1.3.3E"    );
  BOOST_REQUIRE(restore	("clean_session.sql"  )==0);
  BOOST_CHECK	 (connect	(uid  , pwd, key, cop )==0);
  BOOST_CHECK	 (addUser (key  , *use           )==0);
  BOOST_CHECK_THROW	 (changePassword("bad", pass, "newPwd"), VishnuException);
  BOOST_CHECK	 (close         (key                  )==0);

  // Change pwd bad pwd
  BOOST_MESSAGE(" Testing change password bad pwd U1.3.3E"    );
  BOOST_REQUIRE(restore	("clean_session.sql")==0);
  BOOST_CHECK	 (connect	(uid, pwd , key, cop)==0);
  BOOST_CHECK	 (addUser (key, *use           )==0);
  BOOST_CHECK_THROW	 (changePassword(cu, "bad", "newPwd"), VishnuException);
  BOOST_CHECK	 (close         (key                )==0);

//   // Reset pwd ok
//   BOOST_MESSAGE(" Testing reset password normal UA2-B"    );
//   BOOST_REQUIRE(restore      ("clean_session.sql")==0);
//   BOOST_CHECK	 (connect      (uid, pwd, key, cop )==0);
//   BOOST_CHECK    (resetPassword(key, uid            )==0);
//  BOOST_CHECK	 (close        (key                )==0);

  // Reset pwd bad uid
  BOOST_MESSAGE(" Testing reset password bad uid UA2-E"    );
  BOOST_REQUIRE(restore      ("clean_session.sql" )==0);
  BOOST_CHECK	 (connect      (uid, pwd  , key, cop)==0);
  BOOST_CHECK	 (addUser(key, *use            )==0);
  BOOST_CHECK_THROW	 (resetPassword(key, "bad"          ), VishnuException);
  BOOST_CHECK	 (close        (key                 )==0);

  // Add local account
  BOOST_MESSAGE(" Testing add local account success U4-B"    );
  BOOST_REQUIRE(restore("clean_session.sql")==0);
  BOOST_CHECK  (connect(uid, pwd, key, cop )==0);
  lacc.setUserId       (uid);
  lacc.setMachineId    (mid);
  lacc.setAcLogin     (accL);
  lacc.setSshKeyPath   (ssh);
  lacc.setHomeDirectory(home);
  BOOST_CHECK(addLocalAccount(key, lacc, key2)==0);
  BOOST_CHECK(close          (key      )==0);

  // Add local account bad machine id
  BOOST_MESSAGE(" Testing add local account bad machine U4-E"    );
  BOOST_REQUIRE(restore        ("clean_session.sql")==0);
  BOOST_CHECK  (connect        (uid, pwd , key, cop)==0);
  lacc.setMachineId("bad");
  BOOST_CHECK_THROW	 (addLocalAccount(key, lacc, key2  ), VishnuException);
  BOOST_CHECK	 (close          (key        	   )==0);  
  lacc.setMachineId(mid);

  // Add local account bad acc login
  BOOST_MESSAGE(" Testing add local account success U4-E"    );
  BOOST_REQUIRE(restore	 ("clean_session.sql")==0);
  BOOST_CHECK  (connect	 (uid, pwd, key, cop )==0);
  lacc.setAcLogin("bad");
  BOOST_CHECK_THROW	 (addLocalAccount(key, lacc, key2    ), VishnuException);
  BOOST_CHECK	 (close          (key      	     )==0);    
  lacc.setAcLogin(accL);

  // Update local account
  BOOST_MESSAGE(" Testing update local account success U4.1-B"    );
  BOOST_REQUIRE(restore	  ("clean_session.sql")==0);
  BOOST_CHECK  (connect	  (uid, pwd , key, cop)==0);
  lacc.setSshKeyPath("/usr/bin");
  BOOST_CHECK(addLocalAccount   (key, lacc, key2      )==0);    
  BOOST_CHECK(updateLocalAccount(key, lacc	      )==0);
  BOOST_CHECK(close             (key      	      )==0);
  lacc.setSshKeyPath(ssh);

  // Update local account bad machine id
  BOOST_MESSAGE(" Testing update local account bad machine U4.1-E"    );
  BOOST_REQUIRE(restore           ("clean_session.sql")==0);
  BOOST_CHECK  (connect           (uid, pwd , key, cop)==0);
  BOOST_CHECK_THROW	 (addLocalAccount   (key, lacc, key2  	), VishnuException);
  lacc.setMachineId("bad");
  BOOST_CHECK  (updateLocalAccount(key, lacc	        )==0);
  BOOST_CHECK	 (close             (key        	)==0);  
  lacc.setMachineId(mid);

  // Delete local account 
  BOOST_MESSAGE(" Testing delete local account normal U4.2B"    );
  BOOST_REQUIRE(restore           ("clean_session.sql")==0);
  BOOST_CHECK  (connect           (uid, pwd , key, cop)==0);
  BOOST_CHECK	 (addLocalAccount   (key, lacc, key2    )==0);
  BOOST_CHECK	 (deleteLocalAccount(key, uid , mid     )==0);
  BOOST_CHECK	 (close             (key                )==0);  

  // Delete local accountbad uid
  BOOST_MESSAGE(" Testing delete local account bad uid U4.2E"    );
  BOOST_REQUIRE(restore           ("clean_session.sql" )==0);
  BOOST_CHECK  (connect           (uid, pwd  , key, cop)==0);
  BOOST_CHECK	 (addLocalAccount   (key, lacc, key2     )==0);
  BOOST_CHECK_THROW	 (deleteLocalAccount(key, "bad", mid     ), VishnuException);
  BOOST_CHECK	 (close             (key                 )==0);  

  // Delete local accountbad uid
  BOOST_MESSAGE(" Testing delete local account bad machine"    );
  BOOST_REQUIRE(restore           ("clean_session.sql"  )==0);
  BOOST_CHECK  (connect           (uid, pwd , key  , cop)==0);
  BOOST_CHECK	 (addLocalAccount   (key, lacc, key2      )==0);
  BOOST_CHECK_THROW	 (deleteLocalAccount(key, uid , "bad"     ), VishnuException);
  BOOST_CHECK	 (close             (key                  )==0);  

  // Test add machine normal
  BOOST_MESSAGE(" Testing add machine UA6.1B"    );
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  ma.setMachineId         (maid);
  ma.setName              (mana);
  ma.setSite              (site);
  ma.setMachineDescription(desc);
  ma.setLanguage          (lang);
  BOOST_CHECK	 (addMachine (key, ma            )==0);
  BOOST_CHECK	 (close      (key                )==0);  

  // Machine already exist
  BOOST_MESSAGE(" Testing add machine twice the same UA6.1E"    );
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (addMachine (key, ma            )==0);
  BOOST_CHECK_THROW	 (addMachine (key, ma            ), VishnuException);
  BOOST_CHECK	 (close      (key                )==0);  

  // Test update machine normal
  BOOST_MESSAGE(" Testing update machine UA6.4B"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (addMachine   (key, ma            )==0);
  ma.setName("Machina");
  BOOST_CHECK	 (updateMachine(key, ma            )==0);
  BOOST_CHECK	 (close        (key                )==0);  
  ma.setName(mana);

  // Test update machine normal
  BOOST_MESSAGE(" Testing update machine bad machien id UA6.4E"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (addMachine   (key, ma            )==0);
  ma.setMachineId("bad");
  BOOST_CHECK_THROW	 (updateMachine(key, ma            ), VishnuException);
  BOOST_CHECK	 (close        (key                )==0);  
  ma.setMachineId(maid);

  // Test delete machine normal
  BOOST_MESSAGE(" Testing update machine UA6.2B"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd , key, cop)==0);
  BOOST_CHECK	 (addMachine   (key, ma            )==0);
  BOOST_CHECK	 (deleteMachine(key, maid          )==0);
  BOOST_CHECK	 (close        (key                )==0);  

  // Test delete machine normal
  BOOST_MESSAGE(" Testing delete machine bad machine id UA6.2E"    );
  BOOST_REQUIRE(restore      ("clean_session.sql")==0);
  BOOST_CHECK  (connect      (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (addMachine   (key, ma            )==0);
  ma.setMachineId("bad");
  BOOST_CHECK_THROW	 (updateMachine(key, ma            ), VishnuException);
  BOOST_CHECK	 (close        (key                )==0);  
  ma.setMachineId(maid);

  // Test list user
  BOOST_REQUIRE(restore  ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing normal list user UA5.2B" );
  BOOST_CHECK  (connect  (uid, pwd, key, cop )==0);
  BOOST_CHECK  (listUsers(key, *liu, ""       )==0);
  BOOST_CHECK  (close    (key                )==0);
  if (li->getSessions().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((liu->getUsers().size()>0) && (liu->getUsers()[0]->getUserId()!="admin_1" || liu->getUsers()[1]->getUserId()!="user_1"))
    BOOST_MESSAGE(" admin: " << liu->getUsers()[0]->getUserId() << " and user: " << liu->getUsers()[1]->getUserId() );

  // Test list user option user
  BOOST_REQUIRE(restore  ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing list user with login UA5.2B" );
  BOOST_CHECK  (connect  (uid, pwd, key, cop )==0);
  liu = ecoreFactory->createListUsers();
  BOOST_CHECK  (listUsers(key, *liu, "admin_1")==0);
  BOOST_CHECK  (close    (key                )==0);
  if (li->getSessions().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((liu->getUsers().size()>0) && (liu->getUsers()[0]->getUserId()!="admin_1" || liu->getUsers()[1]))
    BOOST_MESSAGE(" FAILED " );

  // Test list user
  BOOST_REQUIRE(restore  ("clean_session.sql"  )==0);
  BOOST_MESSAGE(" Testing bad key list user UA5.2E" );
  BOOST_CHECK  (connect  (uid  , pwd, key, cop )==0);
  liu = ecoreFactory->createListUsers();
  BOOST_CHECK_THROW  (listUsers("bad", *liu, ""       ), VishnuException);
  BOOST_CHECK  (close    (key                  )==0);

  // Test list session
  BOOST_REQUIRE(restore    ("clean_session.sql"  )==0);
  BOOST_MESSAGE(" Testing list session base U1.3.5B" );
  BOOST_CHECK  (connect    (uidu, pwdu, key, cop )==0);
  li = ecoreFactory->createListSessions();
  BOOST_CHECK  (listSessions(key , *li  , opt      )==0);
  BOOST_CHECK  (close      (key                  )==0);
  if (li->getSessions().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((li->getSessions().size()>0) && (li->getSessions()[0]->getSessionKey()!=key))
    BOOST_MESSAGE(" key: " << li->getSessions()[0]->getSessionKey() );

  // Test list session for an admin
  BOOST_REQUIRE(restore    ("clean_session.sql")==0);
  BOOST_MESSAGE(" Testing list session base for admin UA5.1B" );
  li = ecoreFactory->createListSessions();
  BOOST_CHECK  (connect    (uid, pwd, key, cop )==0);
  BOOST_CHECK  (listSessions(key, *li , opt      )==0);
  BOOST_CHECK  (close      (key                )==0);
  if (li->getSessions().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((li->getSessions().size()>0) && (li->getSessions()[0]->getSessionKey()!=key))
    BOOST_MESSAGE(" key: " << li->getSessions()[0]->getSessionKey() );

  // Test list session error
  BOOST_REQUIRE(restore    ("clean_session.sql"   )==0);
  BOOST_MESSAGE(" Testing bad key list session base 1.3.5E" );
  li = ecoreFactory->createListSessions();
  BOOST_CHECK  (connect    (uidu , pwdu, key, cop )==0);
  BOOST_CHECK_THROW  (listSessions("bad", *li  , opt      ), VishnuException);
  BOOST_CHECK  (close      (key                   )==0);

  // Test list session opt
  opt.setUserId(uidu);
  BOOST_REQUIRE(restore    ("clean_session.sql"   )==0);
  BOOST_MESSAGE(" Testing list session base option 1.3.5B" );
  li = ecoreFactory->createListSessions();
  BOOST_CHECK  (connect    (uidu, pwdu, key2, cop )==0);
  BOOST_CHECK  (close      (key2                  )==0);
  BOOST_CHECK  (connect    (uid , pwd	, key , cop )==0);
  BOOST_CHECK  (listSessions(key , *li 	, opt       )==0);
  BOOST_CHECK  (close      (key                   )==0);
  opt  = *(ecoreFactory->createListSessionOptions());
  if (li->getSessions().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((li->getSessions().size()>0) && (li->getSessions()[0]->getSessionKey()!=key2))
    BOOST_MESSAGE(" key: " << li->getSessions()[0]->getSessionKey() );


  // Test list machine
  BOOST_REQUIRE(restore     ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list machine UA6.3B" );
  BOOST_CHECK  (connect     (uid, pwd, key , cop )==0);
  BOOST_CHECK  (listMachine(key, *lim, liom      )==0);
  BOOST_CHECK  (close       (key                 )==0);
  if (lim->getMachines().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((lim->getMachines().size()>0) && (lim->getMachines()[0]->getMachineId()!="machine_1"))
    BOOST_MESSAGE(" machine: " << lim->getMachines()[0]->getMachineId() );

  // Test list machine option mid
  liom.setMachineId("mid");
  BOOST_REQUIRE(restore     ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list machine on a specific machine UA6.3B" );
  lim  = ecoreFactory->createListMachines();
  BOOST_CHECK  (connect     (uid, pwd, key , cop )==0);
  BOOST_CHECK  (listMachine(key, *lim, liom      )==0);
  BOOST_CHECK  (close       (key                 )==0);
  if (lim->getMachines().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((lim->getMachines().size()>0) && (lim->getMachines()[0]->getMachineId()!="machine_1"))
    BOOST_MESSAGE(" machine: " << lim->getMachines()[0]->getMachineId() );

  // Test list machine option bad mid
  liom.setMachineId("bad");
  BOOST_REQUIRE(restore     ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad mid list machine on a specific machine UA6.3E" );
  lim  = ecoreFactory->createListMachines();
  BOOST_CHECK  (connect     (uid, pwd, key , cop )==0);
  BOOST_CHECK_THROW  (listMachine(key, *lim, liom      ), VishnuException);
  BOOST_CHECK  (close       (key                 )==0);

  // Test list local account
  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list local account U4.3B" );
  BOOST_CHECK  (connect         (uid, pwd, key , cop )==0);
  BOOST_CHECK  (listLocalAccount(key, *lia, lioa      )==0);
  BOOST_CHECK  (close           (key                 )==0);
  if (lia->getAccounts().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((lia->getAccounts().size()>0) && (lia->getAccounts()[0]->getMachineId()!="machine_1"))
    BOOST_MESSAGE(" account: " << lia->getAccounts()[0]->getMachineId() );

  // Test list local account mid
  lioa.setMachineId(mid);
  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list local account on a machine U4.3B" );
  BOOST_CHECK  (connect         (uid, pwd, key , cop )==0);
  lia  = ecoreFactory->createListLocalAccounts();
  BOOST_CHECK  (listLocalAccount(key, *lia, lioa      )==0);
  BOOST_CHECK  (close           (key                 )==0);
  if (lia->getAccounts().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((lia->getAccounts().size()>0) && (lia->getAccounts()[0]->getMachineId()!="machine_1"))
    BOOST_MESSAGE(" account: " << lia->getAccounts()[0]->getMachineId() );

  // Test list local account bad mid
  lioa.setMachineId("bad");
  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad machine on list local account U4.3E" );
  BOOST_CHECK  (connect         (uid, pwd, key , cop )==0);
  lia  = ecoreFactory->createListLocalAccounts();
  BOOST_CHECK_THROW  (listLocalAccount(key, *lia, lioa      ), VishnuException);
  BOOST_CHECK  (close           (key                 )==0);

  // Test configure default option
  opva.setOptionName(ona);
  opva.setValue(oval);
  BOOST_REQUIRE(restore               ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list local account UA7-B" );
  BOOST_CHECK  (connect               (uid, pwd , key, cop )==0);
  BOOST_CHECK  (configureDefaultOption(key, opva           )==0);
  BOOST_CHECK  (close                 (key                 )==0);

  // Test configure option
  opva.setOptionName(ona);
  opva.setValue("GLACE");
  BOOST_REQUIRE(restore        ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal configure option U1.3.1-B" );
  BOOST_CHECK  (connect        (uid, pwd , key, cop )==0);
  BOOST_CHECK  (configureOption(key, opva           )==0);
  BOOST_CHECK  (close          (key                 )==0);

  // Test configure option bad option name
  opva.setOptionName("bad");
  opva.setValue     (oval);
  BOOST_REQUIRE(restore        ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad name configure option U1.3.1E1" );
  BOOST_CHECK  (connect        (uid, pwd , key, cop )==0);
  BOOST_CHECK_THROW  (configureOption(key, opva           ), VishnuException);
  BOOST_CHECK  (close          (key                 )==0);

  // Test configure option bad option name
  opva.setOptionName(ona);
  opva.setValue     ("100");
  BOOST_REQUIRE(restore        ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad value configure option U1.3.1E2" );
  BOOST_CHECK  (connect        (uid, pwd , key, cop )==0);
  BOOST_CHECK_THROW  (configureOption(key, opva           ), VishnuException);
  BOOST_CHECK  (close          (key                 )==0);

  // Test list option values
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal list option value U1.3.2B" );
  BOOST_CHECK  (connect    (uid, pwd , key , cop)==0);
  liov  = ecoreFactory->createListOptionsValues();
  BOOST_CHECK  (listOptions(key, *liov, lioo     )==0);
  BOOST_CHECK  (close      (key                 )==0);
  if (liov->getOptionValues().size()==0){
    BOOST_MESSAGE(" Error empty list returned " );
  }
  if ((liov->getOptionValues().size()>0) && (liov->getOptionValues()[0]->getOptionName()!=ona))
    BOOST_MESSAGE(" Option val: " << liov->getOptionValues()[0]->getOptionName() );

  // Test list option values bad option name
  lioo.setOptionName("bad");
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad mid list option value U1.3.2E1" );
  BOOST_CHECK  (connect    (uid, pwd , key , cop)==0);
  liov  = ecoreFactory->createListOptionsValues();
  BOOST_CHECK_THROW  (listOptions(key, *liov, lioo     ), VishnuException);
  BOOST_CHECK  (close      (key                 )==0);

  // Test list option values bad uid
  lioo.setUserId(mid);
  lioo.setUserId("bad");
  BOOST_REQUIRE(restore    ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing bad uid list option value U1.3.2E2" );
  BOOST_CHECK  (connect    (uid, pwd , key , cop)==0);
  liov  = ecoreFactory->createListOptionsValues();
  BOOST_CHECK_THROW  (listOptions(key, *liov, lioo     ), VishnuException);
  BOOST_CHECK  (close      (key                 )==0);

  // Test initialize
  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
  BOOST_MESSAGE(" Testing normal initialize" );
  BOOST_CHECK  (connect         (uid, pwd , key , cop)==0);
  BOOST_CHECK  (vishnuInitialize((char*)path.c_str(), 0, NULL)==0);
  BOOST_CHECK  (close           (key                 )==0);

  // Test initialize bad conf
  BOOST_REQUIRE(restore         ("clean_session.sql"   )==0);
  BOOST_MESSAGE(" Testing initialize bad conf file" );
  BOOST_CHECK  (vishnuInitialize((char *)"bad", 0, NULL        )==0);

//  // Test finalize
//  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
//  BOOST_MESSAGE(" Testing normal finalize" );
//  BOOST_CHECK  (vishnuInitialize((char *)path.c_str(), 0, NULL)==0);
//  BOOST_CHECK  (vishnuFinalize  (                    )==0);
//
//  // Test finalize error 
//  BOOST_REQUIRE(restore         ("clean_session.sql" )==0);
//  BOOST_MESSAGE(" Testing finalize not initialized" );
//  BOOST_CHECK  (vishnuFinalize  (                    ), VishnuException);

  // Test Save configuration
  BOOST_MESSAGE(" Testing save conf"    );
  BOOST_REQUIRE(restore      	   ("clean_session.sql")==0);
  BOOST_CHECK	 (connect      	   (uid, pwd, key, cop )==0);
  BOOST_CHECK	 (saveConfiguration(key, conf          )==0);
  BOOST_CHECK	 (close            (key                )==0);


  // Test restore configuration
  BOOST_MESSAGE(" Testing restore conf"    );
  BOOST_REQUIRE(restore      	      ("clean_session.sql"   )==0);
  BOOST_CHECK	 (connect      	      (uid, pwd  , key  , cop)==0);
  BOOST_CHECK	 (saveConfiguration   (key, conf             )==0);
  BOOST_CHECK	 (restoreConfiguration(key, cpath            )==0);
  BOOST_CHECK	 (close               (key                   )==0);

  // Test restore configuration bad path
  BOOST_MESSAGE(" Testing restore conf"    );
  BOOST_REQUIRE(restore      	      ("clean_session.sql"   )==0);
  BOOST_CHECK	 (connect      	      (uid, pwd  , key  , cop)==0);
  BOOST_CHECK	 (saveConfiguration   (key, conf             )==0);
  //  conf->setFilePath("bad");
  BOOST_CHECK_THROW	 (restoreConfiguration(key, cpath            ), VishnuException);
  BOOST_CHECK	 (close               (key                   )==0);

  // History, make all commands once and test list them
  BOOST_MESSAGE(" Testing history cmd"    );
  BOOST_REQUIRE(restore               ("clean_session.sql" )==0);
  BOOST_CHECK	 (connect               (uid, pwd , key , cop)==0);
  BOOST_CHECK	 (reconnect             (uid, pwd , sid , key)==0);
  BOOST_CHECK	 (listSessions          (key, *li , opt      )==0);
  BOOST_CHECK	 (listUsers             (key, *liu , ""       )==0);
  BOOST_CHECK	 (listMachine           (key, *lim , liom     )==0);
  BOOST_CHECK	 (listLocalAccount      (key, *lia , lioa     )==0);
  BOOST_CHECK	 (listOptions           (key, *liov, lioo     )==0);
  BOOST_CHECK	 (changePassword        (uid, pwd , pwd      )==0);      
  BOOST_CHECK	 (addUser               (key, *use 	     )==0);   
  BOOST_CHECK	 (updateUser            (key, *use 	     )==0);
  BOOST_CHECK	 (deleteUser            (key, cu  	     )==0);            
  BOOST_CHECK	 (deleteMachine         (key, mid	     )==0);
  BOOST_CHECK	 (resetPassword         (key, uid	     )==0);      
  BOOST_CHECK	 (addLocalAccount       (key, lacc, key2     )==0);
  BOOST_CHECK	 (addMachine            (key, ma	     )==0);
  BOOST_CHECK	 (updateMachine         (key, ma	     )==0);
  BOOST_CHECK	 (updateLocalAccount    (key, lacc           )==0);
  BOOST_CHECK	 (deleteLocalAccount    (key, uid , mid      )==0);      
  BOOST_CHECK	 (configureDefaultOption(key, opva	     )==0);
  BOOST_CHECK	 (configureOption       (key, opva	     )==0);            
  BOOST_CHECK	 (resetPassword         (uid, pwd            )==0);
  BOOST_CHECK	 (saveConfiguration     (key, conf           )==0);
  BOOST_CHECK	 (restoreConfiguration  (key, cpath          )==0);
  BOOST_CHECK	 (close                 (key                 )==0); 
  // Listing
  BOOST_CHECK	 (connect               (uid, pwd , key, cop)==0);
  BOOST_CHECK	 (listHistoryCmd        (key, *lic, lico     )==0);
    

}

BOOST_AUTO_TEST_SUITE_END()
