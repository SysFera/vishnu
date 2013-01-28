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




BOOST_FIXTURE_TEST_SUITE( Connect, UMSSeDFixture )

using namespace std;
using namespace UMS_Data;
using namespace vishnu;
namespace bfs= boost::filesystem;

BOOST_AUTO_TEST_CASE( Connect_base )
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
  ListSessionOptions opt;
    
  BOOST_MESSAGE(" Testing normal connection U1-B1" );
  {
    BOOST_CHECK  (connect    (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (listSessions(sess.getSessionKey(), *li , opt      )==0);
    BOOST_MESSAGE("Sess.GetSessionKey() generated : " << sess.getSessionKey() );
    BOOST_CHECK  (close      (sess.getSessionKey()                )==0);

  }
  
  BOOST_MESSAGE(" Testing normal connection U1-B1" );
  {
    User_ptr u1  = ecoreFactory->createUser();
    u1->setUserId   (m_test_ums_admin_vishnu_login)  ;
    u1->setPassword (m_test_ums_admin_vishnu_pwd);
    ListUsers_ptr liuc = ecoreFactory->createListUsers();
    liuc->getUsers().push_back(u1);
    BOOST_CHECK  (connect    (*liuc, sess, cop )==0);
    BOOST_CHECK  (listSessions(sess.getSessionKey(), *li , opt      )==0);
    BOOST_MESSAGE("Sess.GetSessionKey() generated : " << sess.getSessionKey() );
    BOOST_CHECK  (close      (sess.getSessionKey()                )==0);

  }
  
  BOOST_MESSAGE(" Testing normal connection using the .netrc file U1.1-B2" );
  {
    std::string homebefore = getenv("HOME");
    std::string netrcpath;
    bfs::path tempdir = boost::filesystem::temp_directory_path();
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    netrcpath = string(tempdir.c_str()) + "/.netrc";
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu\n" << "login " << m_test_ums_admin_vishnu_login << "\n" << "password " << m_test_ums_admin_vishnu_pwd << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 600 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK  (connect    ("", "", sess, cop )==0);
    BOOST_CHECK  (listSessions(sess.getSessionKey(), *li , opt      )==0);
    BOOST_MESSAGE("Sess.GetSessionKey() generated : " << sess.getSessionKey());
    BOOST_CHECK  (close      (sess.getSessionKey()                )==0);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
      
  }


  // ReConnect normal call
  BOOST_MESSAGE(" Testing normal reconnection U1.5-B-1" );
  {   
    // -> connect
    BOOST_CHECK  (connect    (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_REQUIRE  (listSessions(sess.getSessionKey(), *li , opt      )==0);
    // -> and then reconnect normal
    std::string rec = li->getSessions()[0]->getSessionId();
    BOOST_CHECK(reconnect  (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, rec, sess)==0);
    BOOST_CHECK(listSessions(sess.getSessionKey(), *li , opt     )==0);

  }
}

BOOST_AUTO_TEST_CASE( Connect_failure )
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
  ListSessionOptions opt;
  std::string homebefore = getenv("HOME");
  bfs::path tempdir = boost::filesystem::temp_directory_path();

  
  BOOST_MESSAGE(" Testing with bad uid U1-E1" );
  BOOST_CHECK_THROW  (connect    ("bad", m_test_ums_admin_vishnu_pwd, sess, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(sess.getSessionKey()  , *li , opt     ), VishnuException);
  
  // Connect with bad pwd
  BOOST_MESSAGE(" Testing bad pwd U1-E2");
  BOOST_CHECK_THROW  (connect    (m_test_ums_admin_vishnu_login, "bad", sess, cop), VishnuException);
  BOOST_CHECK_THROW  (listSessions(sess.getSessionKey(), *li   , opt     ), VishnuException);

  // Connect with bad closure policy
  BOOST_MESSAGE(" Testing unknown closure mode U1.1-E1 and U1-E1");
  sct = 3;
  cop.setClosePolicy(sct);
  BOOST_CHECK_THROW  (connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop ), VishnuException);
  BOOST_CHECK_THROW (close(sess.getSessionKey()), VishnuException);
  sct = 0;
  cop.setClosePolicy(sct);

  BOOST_MESSAGE(" Testing connection with bad netrc file path U1.1-E3" );
  BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
  BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
  BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
  
  
  BOOST_MESSAGE(" Testing connection with bad netrc file permissions U1.1-E4" );
  {
    string netrcpath = string(tempdir.c_str()) + "/.netrc";
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu\n" << "login " << m_test_ums_admin_vishnu_login << "\n" << "password " << m_test_ums_admin_vishnu_pwd << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 700 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
  }

  BOOST_MESSAGE(" Testing connection with no vishnu machine on netrc file U1.1-E5" );
  {
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    string netrcpath = string(tempdir.c_str()) + "/.netrc";
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu1\n" << "login " << m_test_ums_admin_vishnu_login << "\n" << "password " << m_test_ums_admin_vishnu_pwd << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 600 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
    
  }

  BOOST_MESSAGE(" Testing connection with no login defined on netrc file U1.1-E6" );
  {
    string netrcpath = string(tempdir.c_str()) + "/.netrc";
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu\n" << "password " << m_test_ums_admin_vishnu_pwd << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 600 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
  }

  BOOST_MESSAGE(" Testing connection with the password defined before the login on netrc file U1.1-E6" );
  {
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    string netrcpath = string(tempdir.c_str()) + "/.netrc";
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu1\n" <<  "password " << m_test_ums_admin_vishnu_pwd << "\n" << "login " << m_test_ums_admin_vishnu_login << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 600 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);

  }

  BOOST_MESSAGE(" Testing connection with no password defined on netrc file U1.1-E7" );
  {
    BOOST_REQUIRE(setenv("HOME", tempdir.c_str(), 1)==0);
    string netrcpath = string(tempdir.c_str()) + "/.netrc";
    std::ofstream f( netrcpath.c_str() );
    f << "machine vishnu1\n" <<  "login " << m_test_ums_admin_vishnu_login << "\n" << std::endl;
    BOOST_REQUIRE (system(string("chmod 600 "+ netrcpath).c_str()) != -1);
    BOOST_CHECK_THROW (connect("", "", sess, cop ), VishnuException);
    bfs::path netrc(netrcpath.c_str());
    BOOST_REQUIRE(bfs::remove_all(netrc)==0);
    BOOST_REQUIRE(setenv("HOME", homebefore.c_str(), 1)==0);
  }

  BOOST_MESSAGE(" Testing connection with a list of user bad user U1.1-E8" );
  {
    ListUsers_ptr liuc  = ecoreFactory->createListUsers();
    User_ptr u1  = ecoreFactory->createUser();
    u1  = ecoreFactory->createUser();
    u1->setUserId   ("bad")  ;
    u1->setPassword (m_test_ums_admin_vishnu_pwd);
    liuc->getUsers().push_back(u1);
    BOOST_CHECK_THROW  (connect    (*liuc, sess, cop ), VishnuException);
  }

  BOOST_MESSAGE(" Testing connection with a list of user bad pwd U1.1-E8" );
  {
    ListUsers_ptr liuc  = ecoreFactory->createListUsers();
    User_ptr u1  = ecoreFactory->createUser();
    u1  = ecoreFactory->createUser();
    u1->setUserId   (m_test_ums_admin_vishnu_login)  ;
    u1->setPassword ("bad");
    liuc->getUsers().push_back(u1);
    BOOST_CHECK_THROW  (connect    (*liuc, sess, cop ), VishnuException);
  }

  
  BOOST_MESSAGE(" Testing close a closed session U1.2-E2");
  BOOST_CHECK_THROW(close(sess.getSessionKey()), VishnuException);

  // Test close with bad sid
  BOOST_MESSAGE(" Testing close a bad session U1.2E1");
  BOOST_CHECK_THROW(close("toto"), VishnuException);

  BOOST_MESSAGE(" Testing error uid reconnect U1.5-E1");
  BOOST_CHECK  (connect    (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
  BOOST_CHECK  (listSessions(sess.getSessionKey(), *li , opt      )==0);
  std::string sid = li->getSessions()[0]->getSessionId();
  BOOST_CHECK_THROW  (reconnect("bad", m_test_ums_admin_vishnu_pwd, sid, sess), VishnuException);
  BOOST_MESSAGE(" Testing error pwd reconnect U1.5-E2");
  BOOST_CHECK_THROW  (reconnect(m_test_ums_admin_vishnu_login, "bad", sid, sess), VishnuException);
  BOOST_CHECK  (close      (sess.getSessionKey()                )==0);

  
}
BOOST_AUTO_TEST_SUITE_END()
