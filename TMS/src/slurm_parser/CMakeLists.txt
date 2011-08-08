cmake_minimum_required(VERSION 2.6)


if (VISHNU_USE_SLURM)

  include_directories(
    ${SLURM_INCLUDE_DIR}
    ${SLURM_PARSER_DIR}
    )

  add_library(utils-slurm STATIC
    opt.c
    slurm_internal_api.c
    uid.c
    slurm_parser.c 
    )

  target_link_libraries(utils-slurm 
    ${SLURM_LIB}
    c
    )

endif (VISHNU_USE_SLURM)
