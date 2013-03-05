include_directories(
    ${VISHNU_SOURCE_DIR}/UMS/src/client
)

include(UnitTest)
unit_test(utilsClientUnitTests vishnu-ums-client)
