#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/format.hpp>
#include "utilClient.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#include "IMSVishnuException.hpp"

BOOST_AUTO_TEST_SUITE( utilClient_unit_tests )

BOOST_AUTO_TEST_CASE( test_raiseCommunicationMsgException_n )
{
  BOOST_REQUIRE_THROW(raiseCommunicationMsgException("youhouuuu"), SystemException)
}

BOOST_AUTO_TEST_CASE( test_raiseExceptionIfNotEmptyMsg_n_emptyMsg )
{
  BOOST_REQUIRE_NO_THROW(raiseExceptionIfNotEmptyMsg(""))
}

BOOST_AUTO_TEST_CASE( test_raiseExceptionIfNotEmptyMsg_n_emptyCode )
{
  // No error code means no throw: "<code>#<message>"
  BOOST_REQUIRE_NO_THROW(raiseExceptionIfNotEmptyMsg("#message sympa"))
}

BOOST_AUTO_TEST_CASE( test_raiseExceptionIfNotEmptyMsg_n_split )
{
  std::string message = "message sympa";
  unsigned int i;
  std::string tmp;

  // Generic error code < 10 -> throws SystemException
  for (i = 0; i < 10; i++) {
    tmp = (boost::format("%1%#%2%") % i % message).str();
    BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg(tmp), SystemException);
  }

  // 10 <= error code < 100 -> throws UMSVishnuException
  for (i = 10; i < 100; i++) {
    tmp = (boost::format("%1%#%2%") % i % message).str();
    BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg(tmp), UMSVishnuException);
  }

  // 100 <= error code < 200 -> throws TMSVishnuException
  for (i = 100; i < 200; i++) {
    tmp = (boost::format("%1%#%2%") % i % message).str();
    BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg(tmp), TMSVishnuException);
  }

  // 200 <= error code < 300 -> throws FMSVishnuException
  for (i = 200; i < 300; i++) {
    tmp = (boost::format("%1%#%2%") % i % message).str();
    BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg(tmp), FMSVishnuException);
  }

  // 300 <= error code < 400 -> throws IMSVishnuException
  for (i = 300; i < 400; i++) {
    tmp = (boost::format("%1%#%2%") % i % message).str();
    BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg(tmp), IMSVishnuException);
  }
}

BOOST_AUTO_TEST_CASE( test_raiseExceptionIfNotEmptyMsg_b_format )
{
  BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg("youhouuuu"), SystemException)
}

BOOST_AUTO_TEST_CASE( test_raiseExceptionIfNotEmptyMsg_b_code )
{
  BOOST_REQUIRE_THROW(raiseExceptionIfNotEmptyMsg("500#message"), SystemException)
}


BOOST_AUTO_TEST_SUITE_END()
