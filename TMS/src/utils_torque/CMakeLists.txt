
include_directories(
  ${TORQUE_INCLUDE_DIR}
)


add_library(utils-torque STATIC
  ck_job_name
  cnt2server
  cvtdate
  get_server
  locate_job
  parse_at
  parse_depend
  parse_destid
  parse_equal
  parse_jobid
  parse_stage
  prepare_path
  prt_job_err
  set_attr
  set_resource
  get_hostaddr
  get_hostname
  net_client
  net_server
  net_set_clse
  port_forwarding
  rm
  pbs_log
  pbs_messages
  setup_env
  pbs_sub
)

target_link_libraries(utils-torque
  ${TORQUE_LIB}
  c
)
