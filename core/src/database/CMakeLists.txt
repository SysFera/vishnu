SET( CMAKE_INSTALL_PREFIX ../../.. )

INCLUDE_DIRECTORIES(
 /usr/include/postgresql 
 ../exception/
)

LINK_DIRECTORIES(
  ../../../lib
)

ADD_EXECUTABLE(mainDB
  main.cpp
  )

ADD_LIBRARY(database-vishnu
  DbFactory.cpp
  Database.cpp
  DatabaseResult.cpp
  POSTGREDatabase.cpp
  )

TARGET_LINK_LIBRARIES(mainDB -lpq exception-vishnu database-vishnu)

install( TARGETS database-vishnu DESTINATION lib )
