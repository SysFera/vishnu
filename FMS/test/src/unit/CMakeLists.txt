
include_directories(
     ${VISHNU_SOURCE_DIR}/core/test/mock/database/
    ${CONFIG_SOURCE_DIR}
    ${REGISTRY_SOURCE_DIR}
    ${UMS_SERVER_SOURCE_DIR}
    ${FMS_SERVER_SOURCE_DIR})
include(UnitTest)
unit_test(ListFileTransfersUnitTests vishnu-core vishnu-core-server-mock vishnu-ums-server-mock mockDb)