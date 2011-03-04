INCLUDE_DIRECTORIES(
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
)

ADD_LIBRARY(exception-vishnu STATIC
 VishnuException.cpp
 SystemException.cpp
 UserException.cpp
 UMSVishnuException.cpp
#  TMSVishnuException.cpp
#  IMSVishnuException.cpp
#  FMSVishnuException.cpp
 InternalVishnuException.cpp
)


# INSTALL( TARGETS exception-vishnu DESTINATION lib )


INSTALL( FILES
    VishnuException.hpp
    UserException.hpp
    SystemException.hpp
    UMSVishnuException.hpp
#     TMSVishnuException.hpp
#     FMSVishnuException.hpp
#     IMSVishnuException.hpp

    DESTINATION
    include
)