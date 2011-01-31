INCLUDE_DIRECTORIES(
 /usr/include/postgresql 
 /home/capo-chichi/Sysfera/vishnu/core/exception#../exception/
)
ADD_LIBRARY(exception-vishnu
 ../exception/VishnuException.cc 
 ../exception/SystemException.cc
)

#TARGET_LINK_LIBRARIES(exception-vishnu 
# /home/capo-chichi/Sysfera/vishnu/core/exception#../exception/
#)

ADD_EXECUTABLE(main 
 main.cc 
 DbFactory.hh DbFactory.cc 
 Database.hh Database.cc 
 DatabaseResult.hh DatabaseResult.cc 
 POSTGREDatabase.hh POSTGREDatabase.cc
)
TARGET_LINK_LIBRARIES(main -lpq exception-vishnu)

#TARGET_LINK_LIBRARIES(ums_interface ${DIET_CLIENT_LIBRARIES}  ${LIBPQ_LIB}  ${LIBCRYPT_LIB})

#ADD_EXECUTABLE(ums_connect ums-connect.c fonction_client.c ${UserManagementSystem_SOURCE_DIR}/src/utils/biblio_error.c)
#TARGET_LINK_LIBRARIES(ums_connect ${DIET_CLIENT_LIBRARIES}  ${LIBPQ_LIB}  ${LIBCRYPT_LIB})
