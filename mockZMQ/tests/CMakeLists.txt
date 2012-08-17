include_directories(
  ${VISHNU_SOURCE_DIR}/mockZMQ
  ${VISHNU_SOURCE_DIR}/mockZMQ/tests
  ${UTILVISHNU_SOURCE_DIR}
  ${EMF_DATA_DIR}
  ${TMS_EMF_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

add_library(fake_zmq
  zmq.hpp
  fake.cpp
)

add_executable(testServer
  TestServer.cpp
  ../Server.cpp
  )
target_link_libraries(testServer
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
)

add_executable(testAnnuary
  TestAnnuary.cpp
  ../Annuary.cpp
  )
target_link_libraries(testAnnuary
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
)

add_executable(testPirate
  TestLazyPirate.cpp
  ../zhelpers.hpp
  )
target_link_libraries(testAnnuary
  ${Boost_LIBRARIES}
  zmq_helper
  vishnu-core
)
