# setup config_tests.h.in
file(TO_NATIVE_PATH ${BIN_DIR} BIN_DIR_NATIVE_PATH)
file(TO_NATIVE_PATH ${CONFIG_DIR} CONFIG_DIR_NATIVE_PATH)
SET(TESTENV_LIBRARY_PATH "${PROJECT_BINARY_DIR}/src/client/:${PROJECT_BINARY_DIR}/src/SeD/:")
if (APPLE)
  SET(TESTENV_LIBRARY_PATH_NAME "DYLD_LIBRARY_PATH")
else (APPLE)
  SET(TESTENV_LIBRARY_PATH_NAME "LD_LIBRARY_PATH")
endif (APPLE)
#
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/diet_config_tests.h.in
  ${PROJECT_BINARY_DIR}/include/diet_config_tests.h)
#
include_directories(${PROJECT_BINARY_DIR}/include/
)

add_library(vishnu-test utils.cpp)
