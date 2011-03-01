INCLUDE_DIRECTORIES(
 ${UTILVISHNU_SOURCE_DIR}
)

ADD_LIBRARY(exception-vishnu
 VishnuException.cpp
 SystemException.cpp
 UserException.cpp
 UMSVishnuException.cpp
#  TMSVishnuException.cpp
#  IMSVishnuException.cpp
#  FMSVishnuException.cpp
 InternalVishnuException.cpp
)


INSTALL( TARGETS exception-vishnu DESTINATION lib )


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