SET( CMAKE_INSTALL_PREFIX ../../.. )

INCLUDE_DIRECTORIES(
 ../exception/
)

LINK_DIRECTORIES(
  ../../../lib
)

ADD_EXECUTABLE(mainReg
  main.cpp
  Mapper_impl.cpp
  Mapper_impl2.cpp
  )

ADD_LIBRARY(register-vishnu
  MapperRegistry.cpp
  Mapper.cpp
  )

TARGET_LINK_LIBRARIES(mainReg register-vishnu exception-vishnu)

install( TARGETS register-vishnu DESTINATION lib )
