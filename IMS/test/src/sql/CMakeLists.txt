# Database initialization for IMS tests
configure_file(IMSinitTest.sql.in "${TEST_SQL_PATH}/IMSinitTest.sql")
configure_file(IMScleanall.sql "${TEST_SQL_PATH}/IMScleanall.sql")
configure_file(IMSTestGetMetric.sql.in "${TEST_SQL_PATH}/IMSTestGetMetric.sql")
configure_file(IMSTestThreshold.sql.in "${TEST_SQL_PATH}/IMSTestThreshold.sql")
configure_file(IMSTestSystemInfo.sql.in "${TEST_SQL_PATH}/IMSTestSystemInfo.sql")
configure_file(IMScleanProcesses.sql.in "${TEST_SQL_PATH}/IMScleanProcesses.sql")

