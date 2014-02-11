#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include "utils.hpp"


BOOST_AUTO_TEST_SUITE( utils_tests)


BOOST_AUTO_TEST_CASE( EmptyJson ) {
  JsonObject o;
  BOOST_REQUIRE_EQUAL(o.encode(), "{}");
}

BOOST_AUTO_TEST_CASE( IdempotentJson ) {
  JsonObject o("{\"a\":1}");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"a\": 1}");
  o.reset("{\"b\": [1,2,3]}");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"b\": [1, 2, 3]}");
}

BOOST_AUTO_TEST_CASE( SetPropertyJson ) {
  JsonObject o;
  o.setProperty("a", "toto");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"a\": \"toto\"}");
}

BOOST_AUTO_TEST_CASE( SetArrayPropertyJson) {
  JsonObject o;
  o.setArrayProperty("b");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"b\": []}");
}

BOOST_AUTO_TEST_CASE( AddItemToLastArrayJson) {
  JsonObject o;
  o.setArrayProperty("b");
  o.addItemToLastArray("gimme");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"b\": [\"gimme\"]}");
  o.addItemToLastArray("that");
  BOOST_REQUIRE_EQUAL(o.encode(), "{\"b\": [\"gimme\", \"that\"]}");
  o.setArrayProperty("c");
  o.addItemToLastArray("!");
  BOOST_REQUIRE_EQUAL(o.encode(),
                      "{\"b\": [\"gimme\", \"that\"], \"c\": [\"!\"]}");
}

BOOST_AUTO_TEST_CASE( DecodeJson ) {
  JsonObject o;
  o.decode("{\"b\": [\"gimme\", \"that\"], \"c\": [\"!\"]}");
  BOOST_REQUIRE_EQUAL(o.encode(),
                      "{\"b\": [\"gimme\", \"that\"], \"c\": [\"!\"]}");
}

BOOST_AUTO_TEST_CASE( GetIntPropertyJson ) {
  JsonObject o("{\"a\": 17}");
  BOOST_REQUIRE_EQUAL(o.getIntProperty("a"), 17);
  BOOST_REQUIRE_EQUAL(o.getIntProperty("b"), -1);
  BOOST_REQUIRE_EQUAL(o.getIntProperty("C", 18), 18);
}

BOOST_AUTO_TEST_CASE( GetStringPropertyJson ) {
  JsonObject o("{\"a\": \"chaud\"}");
  BOOST_REQUIRE_EQUAL(o.getStringProperty("a"), "chaud");
  BOOST_REQUIRE_EQUAL(o.getStringProperty("b"), "");
}

BOOST_AUTO_TEST_CASE( GetArrayPropertyJson ) {
  using boost::assign::list_of;
  JsonObject o("{\"a\": [\"1\", \"2\", \"3\"]}");
  std::vector<std::string> res;
  std::vector<std::string> reference = list_of("1")("2")("3");
  o.getArrayProperty("a", res);
  BOOST_REQUIRE(std::equal(res.begin(), res.end(), reference.begin()));
}




BOOST_AUTO_TEST_SUITE_END()
