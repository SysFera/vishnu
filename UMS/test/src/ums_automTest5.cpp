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

BOOST_AUTO_TEST_CASE( Authentication_base )
{

  Session sess;
  ConnectOptions cop;
  SessionCloseType sct = 1;
  std::string sub  = "";
  cop.setClosePolicy(sct);
  cop.setSessionInactivityDelay(4);
  cop.setSubstituteUserId(sub);
  UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
  AuthSystem asys;
  UMS_Data::EncryptionMethod enc = 0;
  ListAuthSystems_ptr lias  = ecoreFactory->createListAuthSystems();
  AuthAccount aacc;
  ListAuthAccounts_ptr liaa  = ecoreFactory->createListAuthAccounts();
  ListAuthAccOptions liaao;

  BOOST_MESSAGE(" Testing add auth system success UA8"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap1");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
    
  }

  BOOST_MESSAGE(" Testing update auth system  UA8.1"    );
  
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap3");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto3");
    asys.setAuthPassword("toto3");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    asys.setName("ldap4");
    BOOST_CHECK(updateAuthSystem(sess.getSessionKey(), asys)==0);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing list auth system  U5"    );
  {
    ListAuthSysOptions liaso;
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    liaso.setListAllAuthSystems(true);
    liaso.setListFullInfo(true);
    BOOST_CHECK(listAuthSystems(sess.getSessionKey(), *lias, liaso)==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  BOOST_MESSAGE(" Testing add auth account success U6"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap1");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    aacc.setAuthSystemId(asys.getAuthSystemId());
    aacc.setUserId(m_test_ums_admin_vishnu_login);
    aacc.setAcLogin("toto");
    BOOST_CHECK(addAuthAccount(sess.getSessionKey(), aacc)==0);
    BOOST_CHECK(deleteAuthAccount(sess.getSessionKey(), aacc.getAuthSystemId(), aacc.getUserId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing delete auth account success U6.2"    );
  {
    BOOST_CHECK  (connect(uid, pwd, sess, cop )==0);
    aacc.setAuthSystemId(asys.getAuthSystemId());
    aacc.setUserId(m_test_ums_admin_vishnu_login);
    aacc.setAcLogin("toto2");
    BOOST_CHECK(addAuthAccount(sess.getSessionKey(), aacc)==0);
    BOOST_CHECK(deleteAuthAccount(sess.getSessionKey(), aacc.getAuthSystemId(), aacc.getUserId())==0);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing list auth system U6.3"    );
  {
    BOOST_CHECK  (connect(uid, pwd, sess, cop )==0);
    liaao.setListAll(true);
    BOOST_CHECK(listAuthAccounts(sess.getSessionKey(), *liaa, liaao)==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
    
  }
  
}



BOOST_AUTO_TEST_CASE( Authentication_failure )
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
  AuthSystem asys;
  UMS_Data::EncryptionMethod enc = 0;
  ListAuthSystems_ptr lias  = ecoreFactory->createListAuthSystems();
  AuthAccount aacc;
  
  // Add auth system already exist
  BOOST_MESSAGE(" Testing add auth system already exist UA8"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK_THROW(addAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
    
  }
  
  // Add auth system bad type
  BOOST_MESSAGE(" Testing add auth system bad Type UA8"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap2");
    asys.setURI("httm://www.graal.ens-lyon.fr2");
    asys.setAuthLogin("toto2");
    asys.setAuthPassword("toto2");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 7;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base2");
    BOOST_CHECK_THROW(addAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing add auth system with bad LDAP base UA8-E5"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldapBase-add");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    enc = 0;
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setUserPasswordEncryption(enc);
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("base");
    BOOST_CHECK_THROW(addAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  
  BOOST_MESSAGE(" Testing update auth system bad type UA8.1"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap8");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto4");
    asys.setAuthPassword("toto4");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    tsys = 5;
    asys.setType(tsys);
    BOOST_CHECK_THROW(updateAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  
  BOOST_MESSAGE(" Testing update auth system bad status UA8.1"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap3");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto3");
    asys.setAuthPassword("toto3");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 7;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    asys.setStatus(ssys);
    asys.setName("ldap");
    ssys = 7;
    BOOST_CHECK_THROW(updateAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  
  BOOST_MESSAGE(" Testing update auth system with bad LDAP base UA8.1-E5"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldapBase-update");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    enc = 0;
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setUserPasswordEncryption(enc);
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("base");
    BOOST_CHECK_THROW(updateAuthSystem(sess.getSessionKey(), asys), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing delete auth system with bad identifier UA8.2"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW(deleteAuthSystem(sess.getSessionKey(), "bad"), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);

  }
  // List auth system bad user
  BOOST_MESSAGE(" Testing list auth system bad user U5"    );
  {
    ListAuthSysOptions liaso;
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    liaso.setListAllAuthSystems(true);
    liaso.setListFullInfo(true);
    liaso.setUserId("toto");
    BOOST_CHECK_THROW(listAuthSystems(sess.getSessionKey(), *lias, liaso), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  BOOST_MESSAGE(" Testing add auth account success U6"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd,  sess, cop )==0);
    aacc.setAuthSystemId("storm");
    aacc.setUserId(m_test_ums_admin_vishnu_login);
    aacc.setAcLogin("toto");
    BOOST_CHECK_THROW(addAuthAccount(sess.getSessionKey(), aacc), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  // Add auth account bad user
  BOOST_MESSAGE(" Testing add auth account success U6"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    asys.setName("ldap1");
    asys.setURI("httm://www.graal.ens-lyon.fr");
    asys.setAuthLogin("toto");
    asys.setAuthPassword("toto");
    asys.setUserPasswordEncryption(enc);
    AuthType tsys = 0;
    StatusType ssys = 1;
    asys.setType(tsys);
    asys.setStatus(ssys);
    asys.setLdapBase("$USERNAME.base");
    BOOST_CHECK(addAuthSystem(sess.getSessionKey(), asys)==0);
    aacc.setAuthSystemId(asys.getAuthSystemId());
    aacc.setUserId("plop");
    aacc.setAcLogin("toto");
    BOOST_CHECK_THROW(addAuthAccount(sess.getSessionKey(), aacc), VishnuException);
    BOOST_CHECK(deleteAuthSystem(sess.getSessionKey(), asys.getAuthSystemId())==0);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }


  BOOST_MESSAGE(" Testing update auth account bad AuthSystem U6.1"    );
  {
    BOOST_CHECK  (connect(uid, pwd, sess, cop )==0);
    aacc.setUserId(m_test_ums_admin_vishnu_login);
    aacc.setAuthSystemId("toto");
    aacc.setAcLogin("titi");
    BOOST_CHECK_THROW(updateAuthAccount(sess.getSessionKey(), aacc), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  
  // Update auth account bad user
  BOOST_MESSAGE(" Testing update auth account bad userId"    );
  {  
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    aacc.setAuthSystemId(asys.getAuthSystemId());
    aacc.setUserId((m_test_ums_admin_vishnu_login);
    aacc.setAcLogin("toto");
    aacc.setUserId("choco");
    BOOST_CHECK_THROW(updateAuthAccount(sess.getSessionKey(), aacc), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  // Delete auth account bad uid
  BOOST_MESSAGE(" Testing delete auth account bad uid U6.2"    );
  
  BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
  {
    aacc.setUserId("plouf");
    BOOST_CHECK_THROW(deleteAuthAccount(sess.getSessionKey(), aacc.getAuthSystemId(), aacc.getUserId()), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }
  // Delete auth account bad auth id
  BOOST_MESSAGE(" Testing delete auth account bad AuthSystem U6.2"    );
  {
    BOOST_CHECK  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    aacc.setAuthSystemId(asys.getAuthSystemId());
    aacc.setUserId(m_test_ums_admin_vishnu_login);
    aacc.setAuthSystemId("err");
    BOOST_CHECK_THROW(deleteAuthAccount(sess.getSessionKey(), aacc.getAuthSystemId(), aacc.getUserId()), VishnuException);
    BOOST_CHECK(close          (sess.getSessionKey()      )==0);
  }

  
}

BOOST_AUTO_TEST_SUITE_END()