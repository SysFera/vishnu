#
# TMS_Data.cmake
# Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
#
# EMF4CPP is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EMF4CPP is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -funroll-loops")

set(TMS_Data_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataPackageImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataFactoryImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Job.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/JobImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/SubmitOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/SubmitOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobsOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobsOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ProgressOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ProgressOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListProgression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListProgressionImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Progression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ProgressionImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListQueues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListQueuesImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/QueueImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/JobResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/JobResultImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobResults.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobResultsImpl.cpp
   )
   
set(TMS_Data_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data_forward.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataPackage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Job.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobs.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/SubmitOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobsOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ProgressOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListProgression.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Progression.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListQueues.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/JobResult.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobResults.hpp
   )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data_forward.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataFactory.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/TMS_DataPackage.hpp DESTINATION include/emf4cpp/TMS_Data)   
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Job.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobs.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/SubmitOptions.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobsOptions.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ProgressOptions.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListProgression.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Progression.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListQueues.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/Queue.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/JobResult.hpp DESTINATION include/emf4cpp/TMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TMS_Data/ListJobResults.hpp DESTINATION include/emf4cpp/TMS_Data)
   
include_directories(${CMAKE_CURRENT_SOURCE_DIR} /home/capo-chichi/Sysfera/vishnu/core/deps/emf_bin/../include/emf4cpp /home/capo-chichi/Sysfera/vishnu/core/deps/emf_bin/../include/emf4cpp)

add_library(emf4cpp-TMS_Data SHARED ${TMS_Data_HEADERS} ${TMS_Data_SOURCES})
set_target_properties(emf4cpp-TMS_Data PROPERTIES VERSION 0.0.1 SOVERSION 1)

install(TARGETS emf4cpp-TMS_Data DESTINATION lib)

