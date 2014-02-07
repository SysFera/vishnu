#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "DIET_client.cpp"
#include "TMSServices.hpp"
#include "UMSServices.hpp"
#include "FMSServices.hpp"
#include "IMSServices.hpp"


BOOST_AUTO_TEST_SUITE( DIET_client_mock_unit_tests )

BOOST_AUTO_TEST_CASE( my_test_get_n_ums )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_UMS; nb++) {
    BOOST_MESSAGE("Checking module " << SERVICES_UMS[nb]);
    BOOST_REQUIRE_EQUAL(get_module(SERVICES_UMS[nb]), "UMS");
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_n_fms )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_FMS; nb++) {
    BOOST_MESSAGE("Checking module " << SERVICES_FMS[nb]);
    BOOST_REQUIRE_EQUAL(get_module(SERVICES_FMS[nb]), "FMS");
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_n_tms )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_TMS; nb++) {
    BOOST_MESSAGE("Checking module " << SERVICES_TMS[nb]);
    BOOST_REQUIRE_EQUAL(get_module(SERVICES_TMS[nb]), "TMS");
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_n_tms_at )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_TMS; nb++) {
    if (MACHINE_SPECIC_SERVICES_TMS[nb]) {
      BOOST_MESSAGE("Checking module " << SERVICES_TMS[nb] << "@cluster");
      BOOST_REQUIRE_EQUAL(get_module(std::string(SERVICES_TMS[nb]) + "@cluster"), "TMS");
    }
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_n_ims )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_IMS; nb++) {
    BOOST_MESSAGE("Checking module " << SERVICES_IMS[nb]);
    BOOST_REQUIRE_EQUAL(get_module(SERVICES_IMS[nb]), "IMS");
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_n_ims_at )
{
  unsigned int nb;
  for (nb = 0; nb < NB_SRV_IMS; nb++) {
    if (MACHINE_SPECIC_SERVICES_IMS[nb]) {
      BOOST_MESSAGE("Checking module " << SERVICES_IMS[nb] << "@cluster");
      BOOST_REQUIRE_EQUAL(get_module(std::string(SERVICES_IMS[nb]) + "@cluster"), "IMS");
    }
  }
}

BOOST_AUTO_TEST_CASE( my_test_get_b_mod )
{
  BOOST_REQUIRE_EQUAL(get_module("bad"), "");
}

BOOST_AUTO_TEST_CASE( my_test_alloc_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1);
  BOOST_REQUIRE_EQUAL(prof->param_count, 1);
  BOOST_REQUIRE_EQUAL(std::string(prof->name), "alloc");
}

BOOST_AUTO_TEST_CASE( my_test_free_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1);
  BOOST_REQUIRE_EQUAL(prof->param_count, 1);
  BOOST_REQUIRE_EQUAL(std::string(prof->name), "alloc");

  BOOST_REQUIRE_EQUAL(diet_profile_free(prof), 0);
}



BOOST_AUTO_TEST_CASE( my_test_setStr_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1);
  BOOST_REQUIRE_NO_THROW(diet_string_set(prof, 0, "toto"));

  BOOST_REQUIRE_EQUAL(prof->params[0], "toto");
}

BOOST_AUTO_TEST_CASE( my_test_setStr_out_of_bound )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 1);
  BOOST_REQUIRE_THROW(diet_string_set(prof, 10), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_getStr_n )
{
  std::string userId = "";
  diet_profile_t* prof = diet_profile_alloc("alloc", 1);
  diet_string_set(prof, 0, "toto");
  diet_string_get(prof, 0, userId);

  BOOST_REQUIRE_EQUAL(userId, "toto");
}

BOOST_AUTO_TEST_CASE( my_test_serial_n )
{
  diet_profile_t* prof = diet_profile_alloc("alloc", 2);
  diet_string_set(prof, 0, "param1");
  diet_string_set(prof, 1);
  std::string res = my_serialize(prof);
  diet_profile_t* prof2 = my_deserialize(res).get();

  std::string param1;
  std::string param2;
  diet_string_get(prof2, 0, param1);
  diet_string_get(prof2, 1, param2);
  JsonObject jsonProf(res);

  BOOST_REQUIRE_EQUAL(prof->name, jsonProf.getStringProperty("name"));
  BOOST_REQUIRE_EQUAL(prof->name, prof2->name);
  BOOST_REQUIRE_EQUAL(param1, "param1");
  BOOST_REQUIRE_EQUAL(param2, "");
}

BOOST_AUTO_TEST_CASE( my_test_serial_b )
{
  BOOST_REQUIRE_THROW(my_serialize(NULL), SystemException);
}


BOOST_AUTO_TEST_CASE( my_test_deser_n )
{
  std::string profSer = "{\"name\":\"alloc\", \"param_count\":\"1\", \"params\": [\"param1\"]}";
  boost::shared_ptr<diet_profile_t> prof = my_deserialize(profSer);
  BOOST_REQUIRE_EQUAL(prof->name, "alloc");
  BOOST_REQUIRE_EQUAL(prof->param_count, 1);
  BOOST_REQUIRE_EQUAL(prof->params[0], "param1");
}

BOOST_AUTO_TEST_CASE( my_test_deser_b_emp )
{
  std::string profSer;
  BOOST_REQUIRE_THROW(my_deserialize(profSer), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_init_b_nul )
{
  BOOST_REQUIRE_THROW(diet_initialize(NULL, 0, NULL), SystemException);
}

BOOST_AUTO_TEST_CASE( my_test_init_b_invalid )
{
  BOOST_REQUIRE_THROW(diet_initialize("/tmp/toto", 0, NULL), UserException);
}

BOOST_AUTO_TEST_SUITE_END()
