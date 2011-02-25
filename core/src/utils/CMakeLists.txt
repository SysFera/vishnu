
ADD_LIBRARY(utils-vishnu
  utilVishnu.cpp
  )

target_link_libraries(utils-vishnu
crypt)

install( TARGETS utils-vishnu DESTINATION lib )
