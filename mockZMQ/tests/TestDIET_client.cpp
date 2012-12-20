#include <iostream>
#include <vector>
#include "DIET_client.cpp"

class TestDIET_client{
public:
  TestDIET_client(){
  }

  ~TestDIET_client(){
  }

  void
  testGetModuleUMS(){
    BOOST_REQUIRE(get_module("sessionConnect").compare("UMS")==0);
  }
  void
  testGetModuleTMS(){
    BOOST_REQUIRE(get_module("jobSubmit").compare("TMS")==0);
  }
  void
  testGetModuleFMS(){
    BOOST_REQUIRE(get_module("DirRemove").compare("FMS")==0);
  }
  void
  testGetModuleIMS(){
    BOOST_REQUIRE(get_module("int_stop").compare("IMS")==0);
  }
  void
  testGetBadModule(){
    BOOST_REQUIRE(get_module("bad").compare("")==0);
  }
  void
  testDIETAlloc(){
    diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
    BOOST_REQUIRE(prof->IN==1);
    BOOST_REQUIRE(prof->INOUT==2);
    BOOST_REQUIRE(prof->OUT==3);
    BOOST_REQUIRE(std::string(prof->name).compare("alloc")==0);
  }
  void
  testDIET_parameter(){
    diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
    diet_arg_t* res = diet_parameter(prof, 1);
    BOOST_REQUIRE(((diet_profile_t *)res->prof)->IN==1);
    BOOST_REQUIRE(((diet_profile_t *)res->prof)->INOUT==2);
    BOOST_REQUIRE(((diet_profile_t *)res->prof)->OUT==3);
    BOOST_REQUIRE(std::string(((diet_profile_t *)res->prof)->name).compare("alloc")==0);
    BOOST_REQUIRE(res->pos==1);
  }
  void
  testDIET_parameterBad(){
    diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
    BOOST_REQUIRE_THROW(diet_parameter(prof, 10), SystemException);
  }
  void
  testStringSet(){
    diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
    diet_string_set(diet_parameter(prof,0), const_cast<char *>("toto"), DIET_VOLATILE);
  }
  void
  testStringGet(){
    char *userId = NULL;
    diet_profile_t* prof = diet_profile_alloc("alloc", 1, 2, 3);
    diet_string_set(diet_parameter(prof,0), const_cast<char *>("toto"), DIET_VOLATILE);
    diet_string_get(diet_parameter(prof,0), &userId, NULL);
    BOOST_REQUIRE(std::string(userId).compare("toto")==0);
  }
  void
  testMySerialize(){
    diet_profile_t* prof = diet_profile_alloc("alloc", 0, 0, 1);
    diet_string_set(diet_parameter(prof,0), const_cast<char *>("param1"), DIET_VOLATILE);
    diet_string_set(diet_parameter(prof,1), NULL, DIET_VOLATILE);
    std::string res = my_serialize(prof);
    BOOST_REQUIRE(res.compare("alloc$$$0$$$0$$$1$$$param1$$$")==0);
  }
  void
  testMySerializeNULLProfile(){
    BOOST_REQUIRE_THROW(my_serialize(NULL), SystemException);
  }
  void
  testMyDeSerialize(){
    std::string profSer = "alloc$$$0$$$0$$$1$$$param1$$$";
    boost::shared_ptr<diet_profile_t> prof = my_deserialize(profSer);
    BOOST_REQUIRE(std::string(prof.get()->name).compare("alloc")==0);
    BOOST_REQUIRE(prof.get()->IN==0);
    BOOST_REQUIRE(prof.get()->INOUT==0);
    BOOST_REQUIRE(prof.get()->OUT==1);
    BOOST_REQUIRE(std::string(prof.get()->param[0]).compare("param1")==0);
  }
  void
  testMyDeSerializeEmpty(){
    std::string profSer = "";
    BOOST_REQUIRE_THROW(my_deserialize(profSer), SystemException);
  }
  void
  testInitNULL(){
    BOOST_REQUIRE_THROW(diet_initialize(NULL, 0, NULL), SystemException);
  }
  void
  testInitInvalid(){
    BOOST_REQUIRE_THROW(diet_initialize("/tmp/toto", 0, NULL), SystemException);
  }


protected:
private:
};
