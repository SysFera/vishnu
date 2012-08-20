#
# UMS_Data.cmake
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

set(UMS_Data_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataPackageImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataFactoryImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ConnectOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ConnectOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Session.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/SessionImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessionOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessionOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/User.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UserImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsersImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsersOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsersOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/CommandImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCommands.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCommandsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCmdOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCmdOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/LocalAccount.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/LocalAccountImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccounts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccountsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Machine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/MachineImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachines.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachinesImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachineOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachineOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Configuration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ConfigurationImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/OptionValue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/OptionValueImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptionsValues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptionsValuesImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystemImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystemOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystemOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSystemsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSysOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSysOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthAccount.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthAccountImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccounts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccountsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/VersionImpl.cpp
    )

set(UMS_Data_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data_forward.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataPackage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ConnectOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Session.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessionOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/User.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsersOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCommands.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCmdOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/LocalAccount.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccounts.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Machine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachines.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachineOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Configuration.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/OptionValue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptionsValues.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystem.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystemOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSystems.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSysOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthAccount.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccounts.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Version.hpp

    )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataFactory.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/UMS_DataPackage.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ConnectOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Session.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListSessionOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/User.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsers.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListUsersOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Command.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCommands.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListCmdOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/LocalAccount.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccounts.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListLocalAccOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Machine.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachines.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListMachineOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Configuration.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/OptionValue.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptionsValues.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListOptOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystem.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthSystemOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSystems.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthSysOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/AuthAccount.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccounts.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/ListAuthAccOptions.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/UMS_Data/Version.hpp DESTINATION ${INCLUDE_INSTALL_DIR}/emf4cpp/UMS_Data)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${EMF4CPP_INCLUDE_DIR})

add_library(emf4cpp-UMS_Data SHARED ${UMS_Data_HEADERS} ${UMS_Data_SOURCES})
set_target_properties(emf4cpp-UMS_Data PROPERTIES COMPILE_FLAGS "-fPIC")
target_link_libraries(emf4cpp-UMS_Data emf4cpp-ecore emf4cpp-ecorecpp)
set_target_properties(emf4cpp-UMS_Data PROPERTIES VERSION 0.0.1 SOVERSION 1)


install(TARGETS emf4cpp-UMS_Data DESTINATION ${LIB_INSTALL_DIR})
