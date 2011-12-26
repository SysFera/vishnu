include_directories(${Boost_INCLUDE_DIRS}
  config exception utils emfdata database
  ../deps/emf4cpp ${UMS_SERVER_SOURCE_DIR} ${TMS_SERVER_SOURCE_DIR}
  ${IMS_SERVER_SOURCE_DIR} ${FMS_SERVER_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${DATABASE_INCLUDE_DIR})

#################### config ###################################################
set(config_SRCS config/CommonParser.cpp
  config/FileParser.cpp
  config/constants.cpp
  config/ExecConfiguration.cpp)

#################### exceptions ###############################################
set(exception_SRCS exception/VishnuException.cpp
  exception/SystemException.cpp
  exception/UserException.cpp
  exception/UMSVishnuException.cpp
  exception/TMSVishnuException.cpp
  exception/IMSVishnuException.cpp
  exception/FMSVishnuException.cpp
  exception/InternalVishnuException.cpp)

set(exception_HEADERS exception/VishnuException.hpp
  exception/UserException.hpp
  exception/SystemException.hpp
  exception/UMSVishnuException.hpp
  exception/TMSVishnuException.hpp
  exception/FMSVishnuException.hpp
  exception/IMSVishnuException.hpp)

#################### utils ####################################################
set(utils_SRCS utils/utilVishnu.cpp
  utils/utilClient.cpp
  utils/Options.cpp
  utils/sessionUtils.cpp
  utils/CLICmd.cpp
  utils/cliUtil.cpp)

#################### register #################################################
set(registry_SRCS registry/MapperRegistry.cpp
  registry/Mapper.cpp
  registry/UMSMapper.cpp
  registry/FMSMapper.cpp
  registry/IMSMapper.cpp
  registry/TMSMapper.cpp)

#################### emfdata ##################################################
set(UMS_Data_SRCS
  emfdata/UMS_Data.cpp
  emfdata/UMS_Data/UMS_DataPackage.cpp
  emfdata/UMS_Data/UMS_DataFactory.cpp
  emfdata/UMS_Data/UMS_DataPackageImpl.cpp
  emfdata/UMS_Data/UMS_DataFactoryImpl.cpp
  emfdata/UMS_Data/ConnectOptions.cpp
  emfdata/UMS_Data/ConnectOptionsImpl.cpp
  emfdata/UMS_Data/Session.cpp
  emfdata/UMS_Data/SessionImpl.cpp
  emfdata/UMS_Data/ListSessions.cpp
  emfdata/UMS_Data/ListSessionsImpl.cpp
  emfdata/UMS_Data/ListSessionOptions.cpp
  emfdata/UMS_Data/ListSessionOptionsImpl.cpp
  emfdata/UMS_Data/User.cpp
  emfdata/UMS_Data/UserImpl.cpp
  emfdata/UMS_Data/ListUsers.cpp
  emfdata/UMS_Data/ListUsersImpl.cpp
  emfdata/UMS_Data/Command.cpp
  emfdata/UMS_Data/CommandImpl.cpp
  emfdata/UMS_Data/ListCommands.cpp
  emfdata/UMS_Data/ListCommandsImpl.cpp
  emfdata/UMS_Data/ListCmdOptions.cpp
  emfdata/UMS_Data/ListCmdOptionsImpl.cpp
  emfdata/UMS_Data/LocalAccount.cpp
  emfdata/UMS_Data/LocalAccountImpl.cpp
  emfdata/UMS_Data/ListLocalAccounts.cpp
  emfdata/UMS_Data/ListLocalAccountsImpl.cpp
  emfdata/UMS_Data/ListLocalAccOptions.cpp
  emfdata/UMS_Data/ListLocalAccOptionsImpl.cpp
  emfdata/UMS_Data/Machine.cpp
  emfdata/UMS_Data/MachineImpl.cpp
  emfdata/UMS_Data/ListMachines.cpp
  emfdata/UMS_Data/ListMachinesImpl.cpp
  emfdata/UMS_Data/ListMachineOptions.cpp
  emfdata/UMS_Data/ListMachineOptionsImpl.cpp
  emfdata/UMS_Data/Configuration.cpp
  emfdata/UMS_Data/ConfigurationImpl.cpp
  emfdata/UMS_Data/OptionValue.cpp
  emfdata/UMS_Data/OptionValueImpl.cpp
  emfdata/UMS_Data/ListOptionsValues.cpp
  emfdata/UMS_Data/ListOptionsValuesImpl.cpp
  emfdata/UMS_Data/ListOptOptions.cpp
  emfdata/UMS_Data/ListOptOptionsImpl.cpp)

set(UMS_Data_HEADERS
  emfdata/UMS_Data.hpp
  emfdata/UMS_Data_forward.hpp
  emfdata/UMS_Data/UMS_DataPackage.hpp
  emfdata/UMS_Data/UMS_DataFactory.hpp
  emfdata/UMS_Data/ConnectOptions.hpp
  emfdata/UMS_Data/Session.hpp
  emfdata/UMS_Data/ListSessions.hpp
  emfdata/UMS_Data/ListSessionOptions.hpp
  emfdata/UMS_Data/User.hpp
  emfdata/UMS_Data/ListUsers.hpp
  emfdata/UMS_Data/Command.hpp
  emfdata/UMS_Data/ListCommands.hpp
  emfdata/UMS_Data/ListCmdOptions.hpp
  emfdata/UMS_Data/LocalAccount.hpp
  emfdata/UMS_Data/ListLocalAccounts.hpp
  emfdata/UMS_Data/ListLocalAccOptions.hpp
  emfdata/UMS_Data/Machine.hpp
  emfdata/UMS_Data/ListMachines.hpp
  emfdata/UMS_Data/ListMachineOptions.hpp
  emfdata/UMS_Data/Configuration.hpp
  emfdata/UMS_Data/OptionValue.hpp
  emfdata/UMS_Data/ListOptionsValues.hpp
  emfdata/UMS_Data/ListOptOptions.hpp)

set(TMS_Data_SRCS
  emfdata/TMS_Data.cpp
  emfdata/TMS_Data/TMS_DataPackage.cpp
  emfdata/TMS_Data/TMS_DataFactory.cpp
  emfdata/TMS_Data/TMS_DataPackageImpl.cpp
  emfdata/TMS_Data/TMS_DataFactoryImpl.cpp
  emfdata/TMS_Data/Job.cpp
  emfdata/TMS_Data/JobImpl.cpp
  emfdata/TMS_Data/ListJobs.cpp
  emfdata/TMS_Data/ListJobsImpl.cpp
  emfdata/TMS_Data/SubmitOptions.cpp
  emfdata/TMS_Data/SubmitOptionsImpl.cpp
  emfdata/TMS_Data/ListJobsOptions.cpp
  emfdata/TMS_Data/ListJobsOptionsImpl.cpp
  emfdata/TMS_Data/ProgressOptions.cpp
  emfdata/TMS_Data/ProgressOptionsImpl.cpp
  emfdata/TMS_Data/ListProgression.cpp
  emfdata/TMS_Data/ListProgressionImpl.cpp
  emfdata/TMS_Data/Progression.cpp
  emfdata/TMS_Data/ProgressionImpl.cpp
  emfdata/TMS_Data/ListQueues.cpp
  emfdata/TMS_Data/ListQueuesImpl.cpp
  emfdata/TMS_Data/Queue.cpp
  emfdata/TMS_Data/QueueImpl.cpp
  emfdata/TMS_Data/JobResult.cpp
  emfdata/TMS_Data/JobResultImpl.cpp
  emfdata/TMS_Data/ListJobResults.cpp
  emfdata/TMS_Data/ListJobResultsImpl.cpp)
   
set(TMS_Data_HEADERS
  emfdata/TMS_Data.hpp
  emfdata/TMS_Data_forward.hpp
  emfdata/TMS_Data/TMS_DataPackage.hpp
  emfdata/TMS_Data/TMS_DataFactory.hpp
  emfdata/TMS_Data/Job.hpp
  emfdata/TMS_Data/ListJobs.hpp
  emfdata/TMS_Data/SubmitOptions.hpp
  emfdata/TMS_Data/ListJobsOptions.hpp
  emfdata/TMS_Data/ProgressOptions.hpp
  emfdata/TMS_Data/ListProgression.hpp
  emfdata/TMS_Data/Progression.hpp
  emfdata/TMS_Data/ListQueues.hpp
  emfdata/TMS_Data/Queue.hpp
  emfdata/TMS_Data/JobResult.hpp
  emfdata/TMS_Data/ListJobResults.hpp)

set(IMS_Data_SRCS
  emfdata/IMS_Data.cpp
  emfdata/IMS_Data/IMS_DataPackage.cpp
  emfdata/IMS_Data/IMS_DataFactory.cpp
  emfdata/IMS_Data/IMS_DataPackageImpl.cpp
  emfdata/IMS_Data/IMS_DataFactoryImpl.cpp
  emfdata/IMS_Data/Metric.cpp
  emfdata/IMS_Data/MetricImpl.cpp
  emfdata/IMS_Data/ListMetric.cpp
  emfdata/IMS_Data/ListMetricImpl.cpp
  emfdata/IMS_Data/ListProcesses.cpp
  emfdata/IMS_Data/ListProcessesImpl.cpp
  emfdata/IMS_Data/SystemInfo.cpp
  emfdata/IMS_Data/SystemInfoImpl.cpp
  emfdata/IMS_Data/Process.cpp
  emfdata/IMS_Data/ProcessImpl.cpp
  emfdata/IMS_Data/ExportOp.cpp
  emfdata/IMS_Data/ExportOpImpl.cpp
  emfdata/IMS_Data/MetricHistOp.cpp
  emfdata/IMS_Data/MetricHistOpImpl.cpp
  emfdata/IMS_Data/RestartOp.cpp
  emfdata/IMS_Data/RestartOpImpl.cpp
  emfdata/IMS_Data/CurMetricOp.cpp
  emfdata/IMS_Data/CurMetricOpImpl.cpp
  emfdata/IMS_Data/ProcessOp.cpp
  emfdata/IMS_Data/ProcessOpImpl.cpp
  emfdata/IMS_Data/ThresholdOp.cpp
  emfdata/IMS_Data/ThresholdOpImpl.cpp
  emfdata/IMS_Data/ListThreshold.cpp
  emfdata/IMS_Data/ListThresholdImpl.cpp
  emfdata/IMS_Data/Threshold.cpp
  emfdata/IMS_Data/ThresholdImpl.cpp
  emfdata/IMS_Data/SysInfoOp.cpp
  emfdata/IMS_Data/SysInfoOpImpl.cpp
  emfdata/IMS_Data/ListSysInfo.cpp
  emfdata/IMS_Data/ListSysInfoImpl.cpp)

set(IMS_Data_HEADERS
  emfdata/IMS_Data.hpp
  emfdata/IMS_Data_forward.hpp
  emfdata/IMS_Data/IMS_DataPackage.hpp
  emfdata/IMS_Data/IMS_DataFactory.hpp
  emfdata/IMS_Data/Metric.hpp
  emfdata/IMS_Data/ListMetric.hpp
  emfdata/IMS_Data/ListProcesses.hpp
  emfdata/IMS_Data/SystemInfo.hpp
  emfdata/IMS_Data/Process.hpp
  emfdata/IMS_Data/ExportOp.hpp
  emfdata/IMS_Data/MetricHistOp.hpp
  emfdata/IMS_Data/RestartOp.hpp
  emfdata/IMS_Data/CurMetricOp.hpp
  emfdata/IMS_Data/ProcessOp.hpp
  emfdata/IMS_Data/ThresholdOp.hpp
  emfdata/IMS_Data/ListThreshold.hpp
  emfdata/IMS_Data/Threshold.hpp
  emfdata/IMS_Data/SysInfoOp.hpp
  emfdata/IMS_Data/ListSysInfo.hpp
   )
set(FMS_Data_SRCS
  emfdata/FMS_Data.cpp
  emfdata/FMS_Data/FMS_DataPackage.cpp
  emfdata/FMS_Data/FMS_DataFactory.cpp
  emfdata/FMS_Data/FMS_DataPackageImpl.cpp
  emfdata/FMS_Data/FMS_DataFactoryImpl.cpp
  emfdata/FMS_Data/FileStat.cpp
  emfdata/FMS_Data/FileStatImpl.cpp
  emfdata/FMS_Data/CpFileOptions.cpp
  emfdata/FMS_Data/CpFileOptionsImpl.cpp
  emfdata/FMS_Data/LsDirOptions.cpp
  emfdata/FMS_Data/LsDirOptionsImpl.cpp
  emfdata/FMS_Data/MvFileOptions.cpp
  emfdata/FMS_Data/MvFileOptionsImpl.cpp
  emfdata/FMS_Data/LsTransferOptions.cpp
  emfdata/FMS_Data/LsTransferOptionsImpl.cpp
  emfdata/FMS_Data/StopTransferOptions.cpp
  emfdata/FMS_Data/StopTransferOptionsImpl.cpp
  emfdata/FMS_Data/FileTransfer.cpp
  emfdata/FMS_Data/FileTransferImpl.cpp
  emfdata/FMS_Data/FileTransferList.cpp
  emfdata/FMS_Data/FileTransferListImpl.cpp
  emfdata/FMS_Data/HeadOfFileOptions.cpp
  emfdata/FMS_Data/HeadOfFileOptionsImpl.cpp
  emfdata/FMS_Data/TailOfFileOptions.cpp
  emfdata/FMS_Data/TailOfFileOptionsImpl.cpp
  emfdata/FMS_Data/RmFileOptions.cpp
  emfdata/FMS_Data/RmFileOptionsImpl.cpp
  emfdata/FMS_Data/CreateDirOptions.cpp
  emfdata/FMS_Data/CreateDirOptionsImpl.cpp
  emfdata/FMS_Data/DirEntry.cpp
  emfdata/FMS_Data/DirEntryImpl.cpp
  emfdata/FMS_Data/DirEntryList.cpp
  emfdata/FMS_Data/DirEntryListImpl.cpp
    )
   
set(FMS_Data_HEADERS
  emfdata/FMS_Data.hpp
  emfdata/FMS_Data_forward.hpp
  emfdata/FMS_Data/FMS_DataPackage.hpp
  emfdata/FMS_Data/FMS_DataFactory.hpp
  emfdata/FMS_Data/FileStat.hpp
  emfdata/FMS_Data/CpFileOptions.hpp
  emfdata/FMS_Data/LsDirOptions.hpp
  emfdata/FMS_Data/MvFileOptions.hpp
  emfdata/FMS_Data/LsTransferOptions.hpp
  emfdata/FMS_Data/StopTransferOptions.hpp
  emfdata/FMS_Data/FileTransfer.hpp
  emfdata/FMS_Data/FileTransferList.hpp
  emfdata/FMS_Data/HeadOfFileOptions.hpp
  emfdata/FMS_Data/TailOfFileOptions.hpp
  emfdata/FMS_Data/RmFileOptions.hpp
  emfdata/FMS_Data/CreateDirOptions.hpp
  emfdata/FMS_Data/DirEntry.hpp
  emfdata/FMS_Data/DirEntryList.hpp
  )


add_library(vishnu-core ${config_SRCS}
  ${exception_SRCS}
  ${utils_SRCS}
  ${registry_SRCS}
  ${UMS_Data_SRCS} 
  ${TMS_Data_SRCS}
  ${IMS_Data_SRCS}
  ${FMS_Data_SRCS}
)

target_link_libraries(vishnu-core
  emf4cpp-vishnu
  crypt)


set(EMF-ECORE-HEADERS
  ${EMF4CPP_INCLUDE_DIR}/ecore/EAnnotation.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EAttribute.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EClass.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EClassifier.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EcoreFactory.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EcorePackage.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EDataType.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EEnum.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EEnumLiteral.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EFactory.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EGenericType.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EModelElement.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/ENamedElement.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EObject.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EOperation.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EPackage.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EParameter.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EReference.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EStringToStringMapEntry.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/EStructuralFeature.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/ETypedElement.hpp
  ${EMF4CPP_INCLUDE_DIR}/ecore/ETypeParameter.hpp
)

set(EMF-ECORECPP-HEADERS
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping_forward.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/MetaModelRepository.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/notify_forward.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/notify.hpp
)

set(EMF-ECORECPP-MAPPING-HEADERS
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/any.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/any_traits.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/EList.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/EListImpl.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/list.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/out_ptr.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/set_traits.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/string_traits.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/mapping/type_traits.hpp
)

set(EMF-ECORECPP-NOTIFY-HEADERS
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/notify/Adapter.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/notify/Notification.hpp
   ${EMF4CPP_INCLUDE_DIR}/ecorecpp/notify/Notifier.hpp
)

install(TARGETS vishnu-core DESTINATION ${LIB_INSTALL_DIR})
set_target_properties(vishnu-core PROPERTIES VERSION ${VISHNU_VERSION})
install(FILES ${exception_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})

install(FILES emfdata/UMS_Data.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES emfdata/UMS_Data_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${UMS_Data_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/UMS_Data)

install(FILES emfdata/TMS_Data.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES emfdata/TMS_Data_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${TMS_Data_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/TMS_Data)

install(FILES emfdata/FMS_Data.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES emfdata/FMS_Data_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${FMS_Data_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/FMS_Data)

install(FILES emfdata/IMS_Data.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES emfdata/IMS_Data_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR})
install(FILES ${IMS_Data_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/IMS_Data)

install(FILES ${EMF-ECORE-HEADERS}  DESTINATION ${INCLUDE_INSTALL_DIR}/ecore)
install(FILES ${EMF-ECORECPP-HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/ecorecpp)
install(FILES ${EMF-ECORECPP-MAPPING-HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/ecorecpp/mapping)
install(FILES ${EMF-ECORECPP-NOTIFY-HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/ecorecpp/notify)
install(FILES ${EMF4CPP_INCLUDE_DIR}/ecore_forward.hpp DESTINATION ${INCLUDE_INSTALL_DIR})

target_link_libraries(vishnu-core ${Boost_LIBRARIES})

if(NOT CLIENT_ONLY)
  set(database_SRCS 
    database/DbConfiguration.cpp
    database/DbFactory.cpp
    database/Database.cpp
    database/DatabaseResult.cpp)
  set(utils_server_SRCS utils/utilServer.cpp)

  if(MYSQL_FOUND AND ENABLE_MYSQL)
    set(database_SRCS ${database_SRCS} database/MYSQLDatabase.cpp)
    include_directories(${MYSQL_INCLUDE_DIR})
    set(DB_LIBS ${DATABASE_LIBS})
  endif()
   
  if(POSTGRESQL_FOUND AND ENABLE_POSTGRESQL)
    set(database_SRCS ${database_SRCS} database/POSTGREDatabase.cpp)
    include_directories(${DATABASE_INCLUDE_DIR})
    set(DB_LIBS ${DATABASE_LIBS})
  endif()

  add_library(vishnu-core-server ${database_SRCS} ${utils_server_SRCS})
  set_target_properties(vishnu-core-server PROPERTIES VERSION ${VISHNU_VERSION})
  install(TARGETS vishnu-core-server DESTINATION ${LIB_INSTALL_DIR})
  target_link_libraries(vishnu-core-server ${DB_LIBS})

  install(PROGRAMS utils/sendmail.py DESTINATION ${SBIN_INSTALL_DIR})
endif()
