INCLUDE_DIRECTORIES(
 /usr/include/postgresql 
 ../exception/
)
ADD_LIBRARY(exception-vishnu
 ../exception/VishnuException.cc 
 ../exception/SystemException.cc
)

ADD_EXECUTABLE(main 
  main.cc 
  )

ADD_LIBRARY(database-vishnu SHARED
  DbFactory.cc 
  Database.cc 
  DatabaseResult.cc 
  POSTGREDatabase.cc
  )
TARGET_LINK_LIBRARIES(main -lpq exception-vishnu database-vishnu)
