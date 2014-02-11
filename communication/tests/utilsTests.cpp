#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include "DIET_client.h"
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

BOOST_AUTO_TEST_CASE( ProfileSerialization ) {
  diet_profile_t *profile = diet_profile_alloc("tutu", 3);
  diet_string_set(profile, 0, "7");
  diet_string_set(profile, 1, "6");
  diet_string_set(profile, 2, "5");
  BOOST_REQUIRE_EQUAL(JsonObject::serialize(profile),
                      "{\"name\": \"tutu\", \"param_count\": 3, \"params\": [\"7\", \"6\", \"5\"]}");
  diet_profile_free(profile);
}

BOOST_AUTO_TEST_CASE( ProfileDeserialization ) {
  using boost::assign::list_of;
  boost::shared_ptr<diet_profile_t> profile =
    JsonObject::deserialize("{\"name\": \"tutu\", \"param_count\": 3, \"params\": [\"7\", \"6\", \"5\"]}");
  std::vector<std::string> reference = list_of("7")("6")("5");
  const std::vector<std::string>& params = profile->params;
  BOOST_REQUIRE_EQUAL(profile->name, "tutu");
  BOOST_REQUIRE_EQUAL(profile->param_count, 3);
  BOOST_REQUIRE(std::equal(params.begin(), params.end(), reference.begin()));
}



BOOST_AUTO_TEST_SUITE_END()
