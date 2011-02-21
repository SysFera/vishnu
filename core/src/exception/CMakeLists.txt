#SET( CMAKE_INSTALL_PREFIX ../../.. )

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

ADD_EXECUTABLE(mainEx
  main.cpp
  UMS.cpp
  TMS.cpp
  IMS.cpp
  FMS.cpp
  )

target_link_libraries(mainEx
  exception-vishnu)

install( TARGETS exception-vishnu DESTINATION lib )
