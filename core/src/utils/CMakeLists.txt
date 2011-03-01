
INCLUDE_DIRECTORIES(
${DATA_BASE_INCLUDE_DIR}
${POSTGRESQL_INCLUDE_DIR}
${VISHNU_EXCEPTION_INCLUDE_DIR}
)

link_directories(
${DATA_BASE_INCLUDE_DIR}
)

ADD_LIBRARY(utils-vishnu
  utilVishnu.cpp
  )

target_link_libraries(
utils-vishnu
crypt
database-vishnu
exception-vishnu
)

install( TARGETS utils-vishnu DESTINATION lib )
