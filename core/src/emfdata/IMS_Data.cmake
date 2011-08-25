#
# IMS_Data.cmake
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

set(IMS_Data_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataPackageImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataFactoryImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Metric.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/MetricImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListMetric.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListMetricImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListProcesses.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListProcessesImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SystemInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SystemInfoImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Process.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ProcessImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ExportOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ExportOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/MetricHistOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/MetricHistOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/RestartOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/RestartOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/CurMetricOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/CurMetricOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ProcessOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ProcessOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ThresholdOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ThresholdOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListThreshold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListThresholdImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Threshold.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ThresholdImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SysInfoOp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SysInfoOpImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListSysInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListSysInfoImpl.cpp
   )

set(IMS_Data_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data_forward.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataPackage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Metric.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListMetric.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListProcesses.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SystemInfo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Process.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ExportOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/MetricHistOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/RestartOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/CurMetricOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ProcessOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ThresholdOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListThreshold.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Threshold.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SysInfoOp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListSysInfo.hpp
   )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data_forward.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataFactory.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/IMS_DataPackage.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Metric.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListMetric.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListProcesses.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SystemInfo.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Process.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ExportOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/MetricHistOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/RestartOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/CurMetricOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ProcessOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ThresholdOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListThreshold.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/Threshold.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/SysInfoOp.hpp DESTINATION include/emf4cpp/IMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IMS_Data/ListSysInfo.hpp DESTINATION include/emf4cpp/IMS_Data)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${EMF4CPP_INCLUDE_DIR})

add_library(emf4cpp-IMS_Data SHARED  ${IMS_Data_HEADERS} ${IMS_Data_SOURCES})
set_target_properties(emf4cpp-IMS_Data PROPERTIES COMPILE_FLAGS "-fPIC")
target_link_libraries(emf4cpp-IMS_Data emf4cpp-ecore emf4cpp-ecorecpp)
set_target_properties(emf4cpp-IMS_Data PROPERTIES VERSION 0.0.1 SOVERSION 1)

install(TARGETS emf4cpp-IMS_Data DESTINATION lib)