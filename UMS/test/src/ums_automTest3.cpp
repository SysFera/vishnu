#include <boost/test/unit_test.hpp>

// EMF INCLUDE
//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include <iostream>
#include "UMS_fixtures.hpp"
#include "api_ums.hpp"
#include"DIET_client.h"
#include "UMS_test_utils.hpp"




BOOST_FIXTURE_TEST_SUITE( Account, UMSSeDFixture )

using namespace std;
using namespace UMS_Data;
using namespace vishnu;
namespace bfs= boost::filesystem;

BOOST_AUTO_TEST_CASE( Account_base )
{

  Session sess;
  ConnectOptions cop;
  SessionCloseType sct = 1;
  std::string sub  = "";
  cop.setClosePolicy(sct);
  cop.setSessionInactivityDelay(4);
  cop.setSubstituteUserId(sub);
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  ListSessions_ptr li = ecoreFactory->createListSessions();
  PrivilegeType pri = 1;
  User_ptr use = ecoreFactory->createUser();
  std::string cu   = "toto_1";
  std::string pass = "user_1";
  std::string fina = "toto";
  std::string lana = "dupont";
  std::string mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  string accL = "toto"      ;
  string ssh  = "/usr/local";
  string home = "/home/toto";
  LocalAccount lacc;
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);
  std::string key = "";
  Configuration conf ;
  ListLocalAccounts_ptr lia  = ecoreFactory->createListLocalAccounts();
  ListLocalAccOptions lioa ;

 
  BOOST_MESSAGE(" Testing add local account success U4-B"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK(addUser(sess.getSessionKey(), *use)==0);
    lacc.setUserId(use->getUserId());
    lacc.setMachineId(m_test_ums_user_vishnu_machineid);
    lacc.setAcLogin(accL);
    lacc.setSshKeyPath(ssh);
    lacc.setHomeDirectory(home);
    BOOST_CHECK(addLocalAccount(sess.getSessionKey(), lacc, key)==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing update local account success U4.1-B"    );
  {
    BOOST_CHECK  (connect   (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop)==0);
    lacc.setSshKeyPath("/usr/bin");
    BOOST_CHECK(updateLocalAccount(sess.getSessionKey(), lacc           )==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing delete local account normal U4.2B"    );
  { 
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd , sess, cop)==0);
    BOOST_CHECK    (deleteLocalAccount(sess.getSessionKey(), use->getUserId() , m_test_ums_user_vishnu_machineid     )==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close             (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing save conf"    );
  {
    BOOST_CHECK    (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK    (saveConfiguration(sess.getSessionKey(), conf          )==0);
    BOOST_CHECK    (close            (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing restore conf"    );
  {
    BOOST_CHECK    (connect             (m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd  , sess  , cop)==0);
    BOOST_MESSAGE("Configuration = "+conf.getFilePath());
    BOOST_CHECK    (restoreConfiguration(sess.getSessionKey(), conf.getFilePath()            )==0);
    BOOST_CHECK    (changePassword(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, m_test_ums_admin_vishnu_pwd       )==0);
    BOOST_CHECK    (changePassword(m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd , m_test_ums_root_vishnu_pwd       )==0);
    BOOST_CHECK    (close               (sess.getSessionKey()                   )==0);
  }
  
  BOOST_MESSAGE(" Testing normal list local account U4.3B" );
  {
    BOOST_CHECK  (connect         (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (listLocalAccounts(sess.getSessionKey(), *lia, lioa      )==0);
    BOOST_CHECK  (close           (sess.getSessionKey()                 )==0);
    BOOST_CHECK (lia->getAccounts().size()>0);

  }

  // Test list local account mid
  
  
  BOOST_MESSAGE(" Testing normal list local account on a machine U4.3B" );
  BOOST_CHECK  (connect         (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
  lioa.setMachineId(m_test_ums_user_vishnu_machineid);
  lia  = ecoreFactory->createListLocalAccounts();
  BOOST_CHECK  (listLocalAccounts(sess.getSessionKey(), *lia, lioa      )==0);
  BOOST_CHECK  (close           (sess.getSessionKey()                 )==0);
  BOOST_CHECK (lia->getAccounts().size()>0);
  if (lia->getAccounts().size()>0) {
    BOOST_CHECK((lia->getAccounts()[0]->getMachineId()).compare(m_test_ums_user_vishnu_machineid) == 0);
  }
  
}



BOOST_AUTO_TEST_CASE( Account_failure )
{
  Session sess;
  ConnectOptions cop;
  SessionCloseType sct = 1;
  std::string sub  = "";
  cop.setClosePolicy(sct);
  cop.setSessionInactivityDelay(4);
  cop.setSubstituteUserId(sub);
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  ListSessions_ptr li = ecoreFactory->createListSessions();
  PrivilegeType pri = 1;
  User_ptr use = ecoreFactory->createUser();
  std::string cu   = "toto_1";
  std::string pass = "user_1";
  std::string fina = "toto";
  std::string lana = "dupont";
  std::string mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  string accL = "toto"      ;
  string ssh  = "/usr/local";
  string home = "/home/toto";
  LocalAccount lacc;
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);
  std::string key = "";
  Configuration conf ;
  ListLocalAccounts_ptr lia  = ecoreFactory->createListLocalAccounts();
  ListLocalAccOptions lioa ;

  BOOST_MESSAGE(" Testing add local account bad machine U4-E"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK(addUser(sess.getSessionKey(), *use)==0);
    lacc.setUserId(use->getUserId());
    lacc.setAcLogin(accL);
    lacc.setSshKeyPath(ssh);
    lacc.setHomeDirectory(home);
    lacc.setMachineId("bad");
    BOOST_CHECK_THROW      (addLocalAccount(sess.getSessionKey(), lacc, key  ), VishnuException);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close          (sess.getSessionKey()             )==0);
  } 

  BOOST_MESSAGE(" Testing add local account bad user id U4-E"    );
  {
    BOOST_CHECK  (connect  (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    lacc.setUserId("bad");
    lacc.setMachineId(m_test_ums_user_vishnu_machineid);
    BOOST_CHECK_THROW      (addLocalAccount(sess.getSessionKey(), lacc, key    ), VishnuException);
    BOOST_CHECK    (close          (sess.getSessionKey()               )==0);
    
  }

  BOOST_MESSAGE(" Testing update local account bad machine U4.1-E"    );
  {
    BOOST_CHECK  (connect           (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd , sess, cop)==0);
    BOOST_CHECK(addUser(sess.getSessionKey(), *use)==0);
    lacc.setUserId(use->getUserId());
    lacc.setMachineId(m_test_ums_user_vishnu_machineid);
    lacc.setAcLogin(accL);
    lacc.setSshKeyPath(ssh);
    lacc.setHomeDirectory(home);
    BOOST_CHECK    (addLocalAccount   (sess.getSessionKey(), lacc, key   )==0);
    lacc.setMachineId("bad");
    BOOST_CHECK_THROW  (updateLocalAccount(sess.getSessionKey(), lacc             ), VishnuException);
    BOOST_CHECK    (close             (sess.getSessionKey()               )==0);
    lacc.setMachineId(m_test_ums_user_vishnu_machineid);
  }

  BOOST_MESSAGE(" Testing update local account bad user ID U4.1-E"    );
  {
    
    BOOST_CHECK  (connect           (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd , sess, cop)==0);
    lacc.setUserId("bad");
    BOOST_CHECK_THROW  (updateLocalAccount(sess.getSessionKey(), lacc             ), VishnuException);
    BOOST_CHECK    (close             (sess.getSessionKey()               )==0);
  }

  BOOST_MESSAGE(" Testing delete local account bad uid U4.2E"    );
  {
    BOOST_CHECK  (connect           (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd  , sess, cop)==0);
    BOOST_CHECK_THROW      (deleteLocalAccount(sess.getSessionKey(), "bad", m_test_ums_user_vishnu_machineid     ), VishnuException);
    BOOST_CHECK    (close             (sess.getSessionKey()                 )==0);
  }

  BOOST_MESSAGE(" Testing restore conf"    );
  {
    BOOST_CHECK    (connect             (m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd , sess  , cop)==0);
    BOOST_CHECK    (saveConfiguration   (sess.getSessionKey(), conf             )==0);
    //  conf->setFilePath("bad");
    BOOST_CHECK_THROW      (restoreConfiguration(sess.getSessionKey(), "toto"            ), VishnuException);
    BOOST_CHECK    (close               (sess.getSessionKey()                   )==0);
  }

  BOOST_MESSAGE(" Testing bad machine on list local account U4.3E" );
  {
    lioa.setMachineId("bad");
    BOOST_CHECK  (connect         (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    lia  = ecoreFactory->createListLocalAccounts();
    BOOST_CHECK_THROW  (listLocalAccounts(sess.getSessionKey(), *lia, lioa      ), VishnuException);
    BOOST_CHECK  (close           (sess.getSessionKey()                 )==0);
  }
}
BOOST_AUTO_TEST_SUITE_END()
