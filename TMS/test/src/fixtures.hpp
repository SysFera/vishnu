#ifndef FIXTURE_HPP
#define FIXTURE_HPP

#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"


struct VishnuConfigure{

  VishnuConfigure():mac(2){

    mav[0]= (char*)"./tms_automTest";

    mav[1]=getenv("VISHNU_CONFIG_FILE");

   if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    

      BOOST_TEST_MESSAGE( " Vishnu is initialized..." );

  }

~VishnuConfigure(){

  vishnu::vishnuFinalize();
  
  BOOST_TEST_MESSAGE( " Vishnu is finalized..." );
}

int mac;
char* mav[2];
};


#endif
