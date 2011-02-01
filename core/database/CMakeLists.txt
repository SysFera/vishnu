INCLUDE_DIRECTORIES(
 /usr/include/postgresql 
 ../specs/exception/
)
ADD_LIBRARY(exception-vishnu
 ../specs/exception/VishnuException.cc 
 ../specs/exception/SystemException.cc
)

ADD_EXECUTABLE(main 
 main.cc 
 DbFactory.hh DbFactory.cc 
 Database.hh Database.cc 
 DatabaseResult.hh DatabaseResult.cc 
 POSTGREDatabase.hh POSTGREDatabase.cc
)
TARGET_LINK_LIBRARIES(main -lpq exception-vishnu)
