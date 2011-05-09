
include_directories(
  ${TORQUE_INCLUDE_DIR}
)


if (VISHNU_USE_TORQUE)
  add_library(utils-torque STATIC
    pbs_sub
    )

  target_link_libraries(utils-torque
    ${TORQUE_LIB}
    c
    )
endif (VISHNU_USE_TORQUE)
