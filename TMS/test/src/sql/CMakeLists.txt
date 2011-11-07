# Database initialization for TMS tests
configure_file(TMSinitTest.sql.in "${TEST_TMS_SQL}/TMSinitTest.sql")
configure_file(cleanall.sql "${TEST_TMS_SQL}/cleanall.sql")
