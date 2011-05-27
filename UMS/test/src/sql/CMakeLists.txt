# Database initialization for UMS tests
configure_file( UMSinitTest.sql "${TEST_SQL_PATH}/UMSinitTest.sql" COPYONLY)
configure_file( cleanall.sql "${TEST_SQL_PATH}/cleanall.sql" COPYONLY)
configure_file( clean_session.sql "${TEST_SQL_PATH}/clean_session.sql" COPYONLY)

# TODO move to another directory
configure_file(id_rsa.pub "${TEST_SQL_PATH}/id_rsa.pub" COPYONLY)