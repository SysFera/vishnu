set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -funroll-loops")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${EMF4CPP_INCLUDE_DIR})

# add_executable(ums-data-test UMS_Data_test.cpp)
# 
# find_library(EMF4CPP_LIB emf4cpp-ecore
#   /opt/emf4cpp-0.0.2/lib
# )
# find_library(EMF4CPP_CPP_LIB emf4cpp-ecorecpp
#   /opt/emf4cpp-0.0.2/lib
# )
# 
# target_link_libraries(ums-data-test emf4cpp-UMS_Data ${EMF4CPP_LIB} ${EMF4CPP_CPP_LIB})