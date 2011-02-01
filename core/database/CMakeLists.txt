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
 DbFactory.hh DbFactory.cc 
 Database.hh Database.cc 
 DatabaseResult.hh DatabaseResult.cc 
 POSTGREDatabase.hh POSTGREDatabase.cc
)
TARGET_LINK_LIBRARIES(main -lpq exception-vishnu)
