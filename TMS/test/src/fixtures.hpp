#ifndef FIXTURE_HPP
#define FIXTURE_HPP

#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"


struct VishnuConfigure{

  VishnuConfigure():mac(2){

    mav[0]= (char*)"./tms_automTest";

    mav[1]=getenv("VISHNU_CONFIG_FILE");

  /*  if (VishnuTnitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    */

      BOOST_TEST_MESSAGE( " Vishnu is initialized..." );

  }

~VishnuConfigure(){

//VishnuFinalize();
  
  BOOST_TEST_MESSAGE( " Vishnu is finalized..." );
}

int mac;
char* mav[2];
};

template<const char* uid, const char* upwd, const char* mid >
class VishnuConnexion{
  
  public:

//addUser();
//changePassword();
//addMachine();
VishnuConnexion():mmid(mid){
//BOOST_TEST_REQUIRE_EQUAL(connect(uid,upwd,msession,mco),0);
}

~VishnuConnexion(){

}

const UMS_Data::Session msession;
const UMS_Data::ConnectOptions mco;
const std::string mmid;



};
char uid []= "root";
char upwd []= "vishnu_user";
char mid []= "MA_1";

typedef VishnuConnexion<uid,upwd,mid> VishnuConnectParams;



#endif
