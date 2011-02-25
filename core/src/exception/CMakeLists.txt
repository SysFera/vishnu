INCLUDE_DIRECTORIES(
 ${UTILVISHNU_SOURCE_DIR}
)

ADD_LIBRARY(exception-vishnu
 VishnuException.cpp
 SystemException.cpp
 UserException.cpp
 UMSVishnuException.cpp
 TMSVishnuException.cpp
 IMSVishnuException.cpp
 FMSVishnuException.cpp
 InternalVishnuException.cpp
)


install( TARGETS exception-vishnu DESTINATION lib )
