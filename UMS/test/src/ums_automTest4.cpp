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

BOOST_AUTO_TEST_CASE( Machine_base )
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
  string maid = "machine_ums_test";
  string mana = "ulysse"   ;
  string site = "paris"    ;
  string desc = "calcul"   ;
  string lang = "fr"       ;
  Machine ma;
  ListMachines_ptr        lim  = ecoreFactory->createListMachines();
  ListMachineOptions      liom ;
  
  BOOST_MESSAGE(" Testing add machine UA6.1B"    );
  {
    BOOST_CHECK  (connect    (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    ma.setMachineId         (maid);
    ma.setName              (mana);
    ma.setSite              (site);
    ma.setMachineDescription(desc);
    ma.setLanguage          (lang);
    ma.setSshPublicKey      ("id_rsa.pub");
    BOOST_CHECK    (addMachine (sess.getSessionKey(), ma            )==0);
    BOOST_CHECK    (close      (sess.getSessionKey()                )==0);
  }

  BOOST_MESSAGE(" Testing update machine UA6.4B"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    ma.setName("Machina");
    BOOST_CHECK    (updateMachine(sess.getSessionKey(), ma            )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
    
  }
  BOOST_MESSAGE(" Testing update machine UA6.2B"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login,  m_test_ums_admin_vishnu_pwd, sess, cop)==0);
    BOOST_CHECK    (deleteMachine(sess.getSessionKey(), ma.getMachineId()          )==0);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
      
  }

  BOOST_MESSAGE(" Testing normal list machine UA6.3B" );
  {
    BOOST_CHECK  (connect     (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (listMachines(sess.getSessionKey(), *lim, liom      )==0);
    BOOST_CHECK  (close       (sess.getSessionKey()                 )==0);
    BOOST_CHECK (lim->getMachines().size() > 0);
    if (lim->getMachines().size()>0) {
      BOOST_CHECK((lim->getMachines()[0]->getMachineId()).compare(m_test_ums_user_vishnu_machineid) == 0);
    }
  }

  
  BOOST_MESSAGE(" Testing normal list machine on a specific machine UA6.3B" );
  {
    liom.setMachineId(m_test_ums_user_vishnu_machineid);
    lim  = ecoreFactory->createListMachines();
    BOOST_CHECK  (connect     (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK  (listMachines(sess.getSessionKey(), *lim, liom      )==0);
    BOOST_CHECK  (close       (sess.getSessionKey()                 )==0);
    BOOST_CHECK (lim->getMachines().size() > 0);
    if (lim->getMachines().size()>0) {
      BOOST_CHECK((lim->getMachines()[0]->getMachineId()).compare(m_test_ums_user_vishnu_machineid) == 0);
    }
  }
  
}



BOOST_AUTO_TEST_CASE( Machine_failure )
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
  string maid = "machine_ums_test";
  string mana = "ulysse"   ;
  string site = "paris"    ;
  string desc = "calcul"   ;
  string lang = "fr"       ;
  Machine ma;
  ma.setMachineId         (maid);
  ma.setName              (mana);
  ma.setSite              (site);
  ma.setMachineDescription(desc);
  ma.setLanguage          (lang);
  ma.setSshPublicKey      ("id_rsa.pub");
  ListMachines_ptr        lim  = ecoreFactory->createListMachines();
  ListMachineOptions      liom ;


  BOOST_MESSAGE(" Testing update machine bad machine id UA6.4E"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK    (addMachine   (sess.getSessionKey(), ma            )==0);
    ma.setMachineId("bad");
    BOOST_CHECK_THROW      (updateMachine(sess.getSessionKey(), ma            ), VishnuException);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
    ma.setMachineId(maid);
  }
  BOOST_MESSAGE(" Testing delete machine bad machine id UA6.2E"    );
  {
    BOOST_CHECK  (connect      (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    ma.setMachineId("bad");
    BOOST_CHECK_THROW   (deleteMachine(sess.getSessionKey(), ma.getMachineId()          ), VishnuException);
    BOOST_CHECK    (close        (sess.getSessionKey()                )==0);
    ma.setMachineId(maid);
  }

  
  BOOST_MESSAGE(" Testing bad mid list machine on a specific machine UA6.3E" );
  {
    liom.setMachineId("bad");
    BOOST_CHECK  (connect     (m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop )==0);
    BOOST_CHECK_THROW  (listMachines(sess.getSessionKey(), *lim, liom      ), VishnuException);
    BOOST_CHECK  (close       (sess.getSessionKey()                 )==0);
  }
}

BOOST_AUTO_TEST_SUITE_END()