#include <boost/test/unit_test.hpp>
#include <string>

#include "ListFileTransfers.hpp"
#include "DbFactory.hpp"

BOOST_AUTO_TEST_SUITE( ListFileTransfers_unit_tests )

std::string sqlListOfFiles = "SELECT transferId, filetransfer.status, userId, clientMachineId, sourceMachineId, "
"destinationMachineId, sourceFilePath, destinationFilePath, fileSize, startTime,errorMsg,"
" trCommand from filetransfer, vsession "
"where vsession.numsessionid=filetransfer.vsession_numsessionid";

BOOST_AUTO_TEST_CASE( test_processOptions_no_options )
{
  SessionServer session;
  ListFileTransfers listFileTransfers(session);
  std::string test_sql = sqlListOfFiles;
  FMS_Data::LsTransferOptions_ptr options = new FMS_Data::LsTransferOptions;
  listFileTransfers.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfFiles+ " and status='0'");
}

BOOST_AUTO_TEST_CASE( test_processOptions_fromMachineId)
{
  DbFactory factory;
  ExecConfiguration execConfig;
  DbConfiguration dbConfig(execConfig);
  factory.createDatabaseInstance(dbConfig);
  SessionServer session;
  ListFileTransfers listFileTransfers(session);
  std::string test_sql = sqlListOfFiles;
  FMS_Data::LsTransferOptions_ptr options = new FMS_Data::LsTransferOptions;
  options->setFromMachineId("machine1");
  listFileTransfers.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfFiles+ " and (sourceMachineId='machine1' or destinationMachineId='machine1')");
  
}

BOOST_AUTO_TEST_CASE( test_processOptions_bad_status)
{
  SessionServer session;
  ListFileTransfers listFileTransfers(session);
  std::string test_sql = sqlListOfFiles;
  FMS_Data::LsTransferOptions_ptr options = new FMS_Data::LsTransferOptions;
  options->setStatus(-2);
  BOOST_CHECK_THROW(listFileTransfers.processOptions(options, test_sql), UserException);
  
}

BOOST_AUTO_TEST_CASE( test_processOptions_bad_transferId)
{
  SessionServer session;
  ListFileTransfers listFileTransfers(session);
  std::string test_sql = sqlListOfFiles;
  FMS_Data::LsTransferOptions_ptr options = new FMS_Data::LsTransferOptions;
  options->setTransferId("idghdfgfqghdfqdg");
  BOOST_CHECK_THROW(listFileTransfers.processOptions(options, test_sql), UserException);
  
}


BOOST_AUTO_TEST_CASE( test_processOptions_bad_userId)
{
  SessionServer session;
  ListFileTransfers listFileTransfers(session);
  std::string test_sql = sqlListOfFiles;
  FMS_Data::LsTransferOptions_ptr options = new FMS_Data::LsTransferOptions;
  options->setUserId("idghdfgfqghdfqdg");
  BOOST_CHECK_THROW(listFileTransfers.processOptions(options, test_sql), UMSVishnuException);
  
}


BOOST_AUTO_TEST_SUITE_END()