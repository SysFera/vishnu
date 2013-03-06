#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/format.hpp>
#include <cstdlib>
#include "TMSVishnuException.hpp"

#include "ScriptGenConvertor.hpp"

static const std::string generic_Script = std::string("#!/bin/sh\n")+
                                              "#% vishnu_job_name=first_job\n"+
                                              "#% vishnu_output=my_first_job_gen.out\n"+
                                              "#% vishnu_error=my_first_job_gen.err\n"+
                                              "#% vishnu_mailNotification= BEGIN\n"+
                                              "#% vishnu_wallclocklimit=01:00:00\n";


static const std::string torque_Script = std::string("#!/bin/sh\n")+
                                          "#PBS -N first_job\n"+
                                          "#PBS -o my_first_job_gen.out\n"+
                                          "#PBS -e my_first_job_gen.err\n"+
                                          "#PBS -m b\n"+
                                          "#PBS -l walltime=01:00:00\n";

static const std::string ll_Script = std::string("#!/bin/sh\n")+
                                      "# @ job_name=first_job\n"+
                                      "# @ output=my_first_job_gen.out\n"+
                                      "# @ error=my_first_job_gen.err\n"+
                                      "# @ notification=start\n"+
                                      "# @ wall_clock_limit=01:00:00\n"+
                                      "# @ queue";

static const std::string slurm_Script = std::string("#!/bin/sh\n")+
                                      "#SBATCH -J first_job\n"+
                                      "#SBATCH -o my_first_job_gen.out\n"+
                                      "#SBATCH -e my_first_job_gen.err\n"+
                                      "#SBATCH --mail-type=BEGIN\n"+
                                      "#SBATCH -t 01:00:00\n";

static const std::string lsf_Script = std::string("#!/bin/sh\n")+
                                      "#BSUB -J first_job\n"+
                                      "#BSUB -o my_first_job_gen.out\n"+
                                      "#BSUB -e my_first_job_gen.err\n"+
                                      "#% -vishnuMailNofication=BEGIN\n"+
                                      "#% -vishnuWaillClockLimit=01:00:00\n";

static const std::string sge_Script = std::string("#!/bin/sh\n")+
                                      "#$ -N first_job\n"+
                                      "#$ -o my_first_job_gen.out\n"+
                                      "#$ -e my_first_job_gen.err\n"+
                                      "#$ -m b\n"+
                                      "#$ -l s_rt=01:00:00\n";

static const std::string pbs_Script = std::string("#!/bin/sh\n")+
                                      "#PBS -N first_job\n"+
                                      "#PBS -o my_first_job_gen.out\n"+
                                      "#PBS -e my_first_job_gen.err\n"+
                                      "#PBS -m BEGIN\n"+
                                      "#PBS -l walltime=01:00:00\n";

static const std::string deltacloud_Script = std::string("#!/bin/sh\n")+
                                          "#first_job\n"+
                                          "#my_first_job_gen.out\n"+
                                          "#my_first_job_gen.err\n"+
                                          "#BEGIN\n"+
                                          "#01:00:00\n";

static const std::string posix_Script = std::string("#!/bin/sh\n")+
                                              "#% vishnu_job_name=first_job\n"+
                                              "#% vishnu_output=my_first_job_gen.out\n"+
                                              "#% vishnu_error=my_first_job_gen.err\n"+
                                              "##BEGIN\n"+
                                              "#% vishnu_wallclocklimit=01:00:00\n";

static const std::string generic_bad_Script = std::string("#!/bin/sh\n")+
                                              "#s vishnu_job_name first_job\n"+
                                              "#s vishnu_output=my_first_job_gen.out\n"+
                                              "#% vishnu_erro my_first_job_gen.err\n"+
                                              "#s vishnu_mailtoNotification= BEGIN\n"+
                                              "#s vishnu_wallclocklimit=01:00:00\n";
static const std::string generic_bad_Key_Script = std::string("#!/bin/sh\n")+
                                              "#s vishnu_job_name first_job\n"+
                                              "#s vishnu_output=my_first_job_gen.out\n"+
                                              "#% vishnu_bad_key=my_first_job_gen.err\n"+
                                              "#% vishnu_mailtoNotification= BEGIN\n"+
                                              "#% vishnu_bad_key=BEGIN\n"+
                                              "#%vishnu_wallclocklimit=01:00:00\n";
static const std::string notgeneric_Script = std::string("#!/bin/sh\n")+
                                              "#$ -N mySGEjobName\n"+
                                              "#$ -o mySGEJob-$JOB_ID.out\n"+
                                              "#$ -e mySGEJob-$JOB_ID.err\n";


BOOST_AUTO_TEST_SUITE( ScriptGenConvertor_unit_tests )

BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_true )
{
  ScriptGenConvertor scriptGenConvertor(0, generic_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor.scriptIsGeneric(), true);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false )
{
  ScriptGenConvertor scriptGenConvertor_bad(0, notgeneric_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_Torque )
{
  ScriptGenConvertor scriptGenConvertor_bad(0, torque_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_LL )
{
  ScriptGenConvertor scriptGenConvertor_bad(1, ll_Script);;
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_Slurm )
{
  ScriptGenConvertor scriptGenConvertor_bad(2, slurm_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_LSF )
{
  ScriptGenConvertor scriptGenConvertor_bad(3, lsf_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_SGE )
{
  ScriptGenConvertor scriptGenConvertor_bad(4, sge_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_PBSPRO )
{
  ScriptGenConvertor scriptGenConvertor_bad(5, pbs_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}
BOOST_AUTO_TEST_CASE( test_scriptIsGeneric_false_DELTACLOUD )
{
  ScriptGenConvertor scriptGenConvertor_bad(6, deltacloud_Script);
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.scriptIsGeneric(), false);
}


BOOST_AUTO_TEST_CASE( test_parseFile_sucess )
{
  ScriptGenConvertor scriptGenConvertor(0, generic_Script);
  std::string errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
}
BOOST_AUTO_TEST_CASE( test_parseFile_error1 )
{
  ScriptGenConvertor scriptGenConvertor_bad(0, generic_bad_Script);
  std::string errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad.parseFile(errormsg),-1);
  std::string error = "Error : Invalid argument vishnu_erromy_first_job_gen.err at line 4 in your script file\n";
  BOOST_CHECK_EQUAL(errormsg,error);
}
BOOST_AUTO_TEST_CASE( test_parseFile_error2 )
{
  ScriptGenConvertor scriptGenConvertor_bad_key(0, generic_bad_Key_Script);
  std::string errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_bad_key.parseFile(errormsg),-1);
  std::string error = "Error : Invalid argument vishnu_bad_key at line 4 in your script file\n";
  BOOST_CHECK_EQUAL(errormsg,error);

}


BOOST_AUTO_TEST_CASE( test_getConvertedScript_Torque )
{

  std::string errormsg="";
  std::string script="";

  ScriptGenConvertor scriptGenConvertor_Torque(0, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_Torque.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_Torque.getConvertedScript();
  BOOST_CHECK_EQUAL(script,torque_Script);

}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_LL )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_LL(1, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_LL.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_LL.getConvertedScript();
  BOOST_CHECK_EQUAL(script,ll_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_Slurm )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_SLURM(2, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_SLURM.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_SLURM.getConvertedScript();
  BOOST_CHECK_EQUAL(script,slurm_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_LSF )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_LSF(3, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_LSF.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_LSF.getConvertedScript();
  BOOST_CHECK_EQUAL(script,lsf_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_SGE )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_SGE(4, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_SGE.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_SGE.getConvertedScript();
  BOOST_CHECK_EQUAL(script,sge_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_PBSPRO )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_PBSPRO(5, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_PBSPRO.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_PBSPRO.getConvertedScript();
  BOOST_CHECK_EQUAL(script,pbs_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_DELTACLOU )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_DELTACLOUD(6, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_DELTACLOUD.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_DELTACLOUD.getConvertedScript();
  BOOST_CHECK_EQUAL(script,deltacloud_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_POSIX )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_POSIX(7, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_POSIX.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_POSIX.getConvertedScript();
  BOOST_CHECK_EQUAL(script,posix_Script);
}
BOOST_AUTO_TEST_CASE( test_getConvertedScript_UNDEFINED )
{

  std::string errormsg="";
  std::string script="";
  ScriptGenConvertor scriptGenConvertor_UNDEFINED(-1, generic_Script);
  errormsg="";
  BOOST_CHECK_EQUAL(scriptGenConvertor_UNDEFINED.parseFile(errormsg),0);
  BOOST_CHECK(errormsg=="");
  script = scriptGenConvertor_UNDEFINED.getConvertedScript();
  BOOST_CHECK_EQUAL(script,generic_Script);
}



BOOST_AUTO_TEST_SUITE_END()
