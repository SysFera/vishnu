
set(CMAKE_CXX_FLAGS "-Wl,-rpath,.")

FILE(GLOB CMAKES "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake")

foreach(cmake ${CMAKES})
  include(${cmake})
endforeach(cmake)

