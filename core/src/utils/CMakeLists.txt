link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})




ADD_LIBRARY(utils-vishnu
  utilVishnu
  Options
  sessionUtils
  )

target_link_libraries(utils-vishnu
crypt)

install( TARGETS utils-vishnu DESTINATION lib )
