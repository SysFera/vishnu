#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include "DIET_client.h"
#include "utils.hpp"
#include "TMS_Data/Job.hpp"

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

BOOST_AUTO_TEST_CASE( TMS_DataSerialization ) {
  TMS_Data::Job job;
  std::string res = JsonObject::serialize(job);
  BOOST_REQUIRE_EQUAL(res, "{\"outputdir\": \"\", \"errorpath\": \"\", \"submitmachineid\": \"\", \"jobname\": \"\", \"jobid\": \"\", \"owner\": \"\", \"vmIp\": \"\", \"status\": -1, \"submitmachinename\": \"\", \"vsession\": \"\", \"jobqueue\": \"\", \"jobprio\": -1, \"submitdate\": -1, \"batchjobid\": \"\", \"jobpath\": \"\", \"outputpath\": \"\", \"enddate\": -1, \"nbcpus\": -1, \"userid\": \"\", \"nbnodes\": -1, \"jobworkingdir\": \"\", \"wallclocklimit\": -1, \"groupname\": \"\", \"jobdescription\": \"\", \"nbnodesandcpupernode\": \"\", \"memlimit\": -1, \"vmId\": \"\"}");
  job.setJobName("toto");
  job.setJobId("J_3");
  job.setJobPath("/home/georges/bin/myjob.sh");
  job.setOutputPath("/home/georges/outputs");
  job.setErrorPath("/home/georges/errors");
  job.setJobWorkingDir("/home/georges/workingdir");
  job.setNbCpus(8);
  job.setNbNodes(3);
  job.setUserId("U_7");
  job.setSubmitDate(1392201353); //  02/12/14 @ 10:35:53am UTC
  BOOST_REQUIRE_EQUAL(JsonObject::serialize(job),
      "{\"outputdir\": \"\", \"errorpath\": \"/home/georges/errors\", \"submitmachineid\": \"\", \"jobname\": \"toto\", \"jobid\": \"J_3\", \"owner\": \"\", \"vmIp\": \"\", \"status\": -1, \"submitmachinename\": \"\", \"vsession\": \"\", \"jobqueue\": \"\", \"jobprio\": -1, \"submitdate\": 1392201353, \"batchjobid\": \"\", \"jobpath\": \"/home/georges/bin/myjob.sh\", \"outputpath\": \"/home/georges/outputs\", \"enddate\": -1, \"nbcpus\": 8, \"userid\": \"U_7\", \"nbnodes\": 3, \"jobworkingdir\": \"/home/georges/workingdir\", \"wallclocklimit\": -1, \"groupname\": \"\", \"jobdescription\": \"\", \"nbnodesandcpupernode\": \"\", \"memlimit\": -1, \"vmId\": \"\"}");
}

BOOST_AUTO_TEST_CASE( TMS_DataDeserialization ) {
  JsonObject o("{\"outputdir\": \"\", \"errorpath\": \"/home/georges/errors\", \"submitmachineid\": \"\", \"jobname\": \"toto\", \"jobid\": \"J_3\", \"owner\": \"\", \"vmIp\": \"\", \"status\": -1, \"submitmachinename\": \"\", \"vsession\": \"\", \"jobqueue\": \"\", \"jobprio\": -1, \"submitdate\": 1392201353, \"batchjobid\": \"\", \"jobpath\": \"/home/georges/bin/myjob.sh\", \"outputpath\": \"/home/georges/outputs\", \"enddate\": -1, \"nbcpus\": 8, \"userid\": \"U_7\", \"nbnodes\": 3, \"jobworkingdir\": \"/home/georges/workingdir\", \"wallclocklimit\": -1, \"groupname\": \"\", \"jobdescription\": \"\", \"nbnodesandcpupernode\": \"\", \"memlimit\": -1, \"vmId\": \"\"}");
  TMS_Data::Job job = o.getJob();
  BOOST_REQUIRE_EQUAL(job.getJobName(), "toto");
  BOOST_REQUIRE_EQUAL(job.getJobId(), "J_3");
  BOOST_REQUIRE_EQUAL(job.getJobPath(), "/home/georges/bin/myjob.sh");
  BOOST_REQUIRE_EQUAL(job.getOutputPath(), "/home/georges/outputs");
  BOOST_REQUIRE_EQUAL(job.getErrorPath(), "/home/georges/errors");
  BOOST_REQUIRE_EQUAL(job.getJobWorkingDir(), "/home/georges/workingdir");
  BOOST_REQUIRE_EQUAL(job.getNbCpus(), 8);
  BOOST_REQUIRE_EQUAL(job.getNbNodes(), 3);
  BOOST_TEST_MESSAGE(job.getUserId());
  BOOST_REQUIRE_EQUAL(job.getUserId(), "U_7");
  BOOST_REQUIRE_EQUAL(job.getSubmitDate(), 1392201353);
}

BOOST_AUTO_TEST_SUITE_END()
