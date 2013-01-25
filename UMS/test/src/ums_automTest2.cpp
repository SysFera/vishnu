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




BOOST_FIXTURE_TEST_SUITE( User, UMSSeDFixture )

using namespace std;
using namespace UMS_Data;
using namespace vishnu;
namespace bfs= boost::filesystem;

BOOST_AUTO_TEST_CASE( User_base )
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
  ListUsers_ptr liu  = ecoreFactory->createListUsers();
  ListUsersOptions liuo;
  PrivilegeType pri = 1;
  std::string cu   = "toto_1";
  std::string pass = "user_1";
  std::string fina = "toto";
  std::string lana = "dupont";
  std::string mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  User_ptr use = ecoreFactory->createUser();
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);

  BOOST_MESSAGE(" Testing adding a user UA4-B");
  {
    BOOST_CHECK(connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop)==0);
    BOOST_CHECK(addUser(sess.getSessionKey(), *use)==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK(close        (sess.getSessionKey()     )==0);
  }
  
  BOOST_MESSAGE(" Testing valid update user UA4.1-B"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (addUser(sess.getSessionKey(), *use           )==0);
    use->setEmail("cl3m3ntlebgkidechyrGRAVE@hotmail.fr");
    BOOST_CHECK (updateUser   (sess.getSessionKey(), *use           )==0);
    use->setEmail(mail);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
   
  }

  
  BOOST_MESSAGE(" Testing delete normal UA4.2B"    );
  {
    BOOST_CHECK    (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    use->setUserId   (cu)  ;
    use->setPassword (pass);
    use->setFirstname(fina);
    use->setLastname (lana);
    use->setPrivilege(pri) ;
    use->setEmail    (mail);
    BOOST_CHECK    (addUser(sess.getSessionKey(), *use       )==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing normal list user UA5.2B" );
  {
    
    liuo.setUserId ("");
    BOOST_CHECK  (connect  (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (listUsers(sess.getSessionKey(), *liu, liuo       )==0);
    BOOST_CHECK  (close    (sess.getSessionKey()                )==0);
    BOOST_CHECK (liu->getUsers().size()>0);
    BOOST_CHECK (liu->getUsers()[0]->getUserId() == "admin_1");
  }

  if (m_test_ums_authen_type.compare("LDAP") != 0) {
    BOOST_MESSAGE(" Testing change password normal U1.3.3"    );
    {
      std::string uid = m_test_ums_admin_vishnu_login;
      std::string pwd = m_test_ums_admin_vishnu_login;
      BOOST_CHECK  (connect       (uid, pwd , sess, cop)==0);
      BOOST_CHECK  (changePassword(uid , pwd, "newPwd")==0);
      BOOST_CHECK  (changePassword(uid , "newPwd", pwd)==0);
      BOOST_CHECK  (close         (sess.getSessionKey()                )==0);
    }
    // Reset pwd ok
    BOOST_MESSAGE(" Testing reset password normal UA2-B"    );
    {
      std::string uid = m_test_ums_admin_vishnu_login;
      std::string pwd = m_test_ums_admin_vishnu_login;
      std::string np;
      BOOST_CHECK  (connect      (uid, pwd, sess, cop )==0);
      BOOST_CHECK    (resetPassword(sess.getSessionKey(), uid, np       )==0);
      BOOST_CHECK    (changePassword(uid, np, pwd       )==0);
      BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
    }
  } 
  
}



BOOST_AUTO_TEST_CASE( User_failure )
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
  ListUsers_ptr liu  = ecoreFactory->createListUsers();
  ListUsersOptions liuo;
  PrivilegeType pri = 1;
  std::string cu   = "toto_1";
  std::string pass = "user_1";
  std::string fina = "toto";
  std::string lana = "dupont";
  std::string mail = "cl3m3ntlebgkidechyr@hotmail.fr";
  User_ptr use = ecoreFactory->createUser();
  use->setUserId   (cu)  ;
  use->setPassword (pass);
  use->setFirstname(fina);
  use->setLastname (lana);
  use->setPrivilege(pri) ;
  use->setEmail    (mail);
  
  // Not admin
  BOOST_MESSAGE(" Testing adding whereas not admin UA4-E"    );
  {
    BOOST_CHECK    (connect      (m_test_ums_user_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW      (addUser(sess.getSessionKey(), *use           ), VishnuException);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
  }
  // Add user bad email
  BOOST_MESSAGE(" Testing adding a user with a bad email UA4-E"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    use->setEmail("cl3m3ntlebgkidechyrhotmail.fr");
    BOOST_CHECK_THROW  (addUser(sess.getSessionKey(), *use           ), VishnuException);
    BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
    use->setEmail(mail);
    
  }

  // Update not an admin
  BOOST_MESSAGE(" Testing update a user with a non admi user UA4.1-E3"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (addUser(sess.getSessionKey(), *use           )==0);
    BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
    use->setEmail("cl3m3ntlebgkidechyrGRAVE@hotmail.fr");
    BOOST_CHECK  (connect      (m_test_ums_user_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW (updateUser   (sess.getSessionKey(), *use           ), VishnuException);
    BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    use->setEmail(mail);
    BOOST_CHECK  (close        (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing bad update parameter bad user id UA4.1-E1"    );
  {
    
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop)==0);
    BOOST_CHECK    (addUser(sess.getSessionKey(), *use      )==0);
    use->setUserId("bad");
    BOOST_CHECK_THROW      (updateUser   (sess.getSessionKey(), *use        ), VishnuException);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()               )==0);
    use->setUserId(cu);
    
  }

  BOOST_MESSAGE(" Testing bad update parameter no session UA4.1-E2"    );
  {
    
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop)==0);
    BOOST_CHECK    (addUser(sess.getSessionKey(), *use      )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()               )==0);    
    BOOST_CHECK_THROW      (updateUser   (sess.getSessionKey(), *use        ), VishnuException);
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop)==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()               )==0);
  }

  BOOST_MESSAGE(" Testing delete normal UA4.2B"    );
  {
    BOOST_CHECK    (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    use->setUserId   (cu)  ;
    use->setPassword (pass);
    use->setFirstname(fina);
    use->setLastname (lana);
    use->setPrivilege(pri) ;
    use->setEmail    (mail);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
    BOOST_CHECK    (addUser(sess.getSessionKey(), *use       )==0);
    BOOST_CHECK    (connect      (m_test_ums_user_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW      (deleteUser   (sess.getSessionKey(), use->getUserId()     ), VishnuException);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
    BOOST_CHECK    (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing delete bad uid UA4.2E"    );
  {
    BOOST_CHECK    (connect   (m_test_ums_admin_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW      (deleteUser(sess.getSessionKey(), cu            ), VishnuException);
    BOOST_CHECK    (close     (sess.getSessionKey()                )==0);
  }


  
  BOOST_MESSAGE(" Testing error list user bad authsysid UA5.2E" );
  {
    liuo.setUserId ("");
    liuo.setAuthSystemId ("lapin");
    BOOST_CHECK  (connect  (m_test_ums_admin_vishnu_login,m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW  (listUsers(sess.getSessionKey(), *liu, liuo       ), VishnuException);
    BOOST_CHECK  (close    (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing bad key list user UA5.2E" );
  {
    liuo.setUserId ("");
    BOOST_CHECK  (connect  (m_test_ums_admin_vishnu_login  , m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    liu = ecoreFactory->createListUsers();
    BOOST_CHECK_THROW  (listUsers("bad", *liu, liuo       ), VishnuException);
    BOOST_CHECK  (close    (sess.getSessionKey()                  )==0);
  }

  if (m_test_ums_authen_type.compare("LDAP") != 0) {
    std::string uid = m_test_ums_admin_vishnu_login;
    std::string pwd = m_test_ums_admin_vishnu_pwd;
   
    // Change pwd bad uid
    BOOST_MESSAGE(" Testing change password bad uid U1.3.3E"    );
    {
      BOOST_CHECK  (connect       (uid  , pwd, sess, cop )==0);
      BOOST_CHECK  (addUser (sess.getSessionKey()  , *use           )==0);
      BOOST_CHECK_THROW    (changePassword("bad", pass, "newPwd"), VishnuException);
      
      BOOST_CHECK  (close         (sess.getSessionKey()                  )==0);
     }
    // Change pwd bad pwd
    BOOST_MESSAGE(" Testing change password bad pwd U1.3.3E"    );
    {
      BOOST_CHECK  (connect       (uid, pwd , sess, cop)==0);
      BOOST_CHECK  (addUser (sess.getSessionKey(), *use           )==0);
      BOOST_CHECK_THROW    (changePassword(use->getUserId(), "bad", "newPwd"), VishnuException);
      BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
      BOOST_CHECK  (close         (sess.getSessionKey()                )==0);
    }

    
    // Reset pwd bad uid
    
    BOOST_MESSAGE(" Testing reset password bad uid UA2-E"    );
    {
      std::string np;
      BOOST_CHECK  (connect      (uid, pwd  , sess, cop)==0);
      BOOST_CHECK  (addUser(sess.getSessionKey(), *use            )==0);
      BOOST_CHECK_THROW    (resetPassword(sess.getSessionKey(), "bad", np          ), VishnuException);
      BOOST_CHECK    (deleteUser   (sess.getSessionKey(), use->getUserId()     )==0);
      BOOST_CHECK  (close        (sess.getSessionKey()                 )==0);
    }
    
  }

  
  
}
BOOST_AUTO_TEST_SUITE_END()
