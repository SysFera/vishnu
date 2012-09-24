set(TORQUESERVER "")
set(TORQUE_ALL_INCLUDE_DIR "")
set(TORQUE_ALL_LIB_DIR "")

set(PBSPROSERVER "")
set(PBSPRO_ALL_INCLUDE_DIR "")
set(PBSPRO_ALL_LIB_DIR "")

set(LOADLEVELERSERVER "")
set(LOADLEVELER_ALL_INCLUDE_DIR "")
set(LOADLEVELER_ALL_LIB_DIR "")

set(SLURMSERVER "")
set(SLURM_ALL_INCLUDE_DIR "")
set(SLURM_ALL_LIB_DIR "")
set(SGESERVER "")
set(SGE_ALL_INCLUDE_DIR "")
set(SGE_ALL_LIB_DIR "")

set(LSFSERVER "")
set(LSF_ALL_INCLUDE_DIR "")
set(LSF_ALL_LIB_DIR "")

if (pbs)
  set(PBSPROSERVER server/PbsProServer.cpp utils_pbs/pbs_sub.c)
  set(PBSPRO_ALL_INCLUDE_DIR ${PBSPRO_INCLUDE_DIR}  ${UTILS_PBSPRO_DIR})
  set(PBSPRO_ALL_LIB_DIR ${PBSPRO_LIB})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_pbs/pbs_server_config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_pbs/pbs_server_config.h)
endif(pbs)

if (torque)
  set(TORQUESERVER server/TorqueServer.cpp utils_torque/pbs_sub.c)
  set(TORQUE_ALL_INCLUDE_DIR ${TORQUE_INCLUDE_DIR}  ${UTILS_TORQUE_DIR})
  set(TORQUE_ALL_LIB_DIR ${TORQUE_LIB})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_torque/pbs_server_config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_torque/pbs_server_config.h)
endif(torque)

if(ll)
  set(LOADLEVELERSERVER server/LLServer.cpp)
  set(LOADLEVELER_ALL_INCLUDE_DIR ${LOADLEVELER_INCLUDE_DIR})
  set(LOADLEVELER_ALL_LIB_DIR ${LOADLEVELER_LIB})
endif(ll) 

if (slurm)
  set(SLURMSERVER server/SlurmServer.cpp
    slurm_parser/opt.c
    slurm_parser/slurm_internal_api.c
    slurm_parser/uid.c
    slurm_parser/slurm_parser.c)
  set(SLURM_ALL_INCLUDE_DIR ${SLURM_INCLUDE_DIR}  ${SLURM_PARSER_DIR})
  set(SLURM_ALL_LIB_DIR ${SLURM_LIB})
endif(slurm)

if(lsf)
  set(LSFSERVER server/LSFServer.cpp 
      lsf_parser/LSFParser.cpp)
  set(LSF_ALL_INCLUDE_DIR ${LSF_INCLUDE_DIR} ${LSF_PARSER_DIR})
  set(LSF_ALL_LIB_DIR ${LSF_LIB} ${LSBATCH_LIB} nsl)
endif(lsf) 

if (sge)
  set(SGESERVER server/SGEServer.cpp )
  set(SGE_ALL_INCLUDE_DIR ${SGE_INCLUDE_DIR})
  set(SGE_ALL_LIB_DIR ${SGE_LIB})
endif(sge)

set(UsedBatchServer ${TORQUESERVER}  ${LOADLEVELERSERVER}  ${SLURMSERVER} ${SGESERVER} ${LSFSERVER}  ${PBSPROSERVER})
set(USED_BATCH_INCLUDE_DIR  ${TORQUE_ALL_INCLUDE_DIR}  ${LOADLEVELER_ALL_INCLUDE_DIR}  ${SLURM_ALL_INCLUDE_DIR} ${SGE_ALL_INCLUDE_DIR} ${LSF_ALL_INCLUDE_DIR} ${PBSPRO_ALL_INCLUDE_DIR})
set(USED_BATCH_LIB ${TORQUE_ALL_LIB_DIR}  ${LOADLEVELER_ALL_LIB_DIR}  ${SLURM_ALL_LIB_DIR} ${SGE_ALL_LIB_DIR} ${LSF_ALL_LIB_DIR} ${PBSPRO_ALL_LIB_DIR})
