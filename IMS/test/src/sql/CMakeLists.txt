# Database initialization for IMS tests
configure_file( IMSinitTest.sql.in "${TEST_IMS_SQL}/IMSinitTest.sql" )
configure_file( IMScleanall.sql "${TEST_IMS_SQL}/IMScleanall.sql" )
configure_file( IMSTestGetMetric.sql.in "${TEST_IMS_SQL}/IMSTestGetMetric.sql" )
