cmake_minimum_required(VERSION 2.6)


if (VISHNU_USE_SLURM)

  include_directories(
    ${SLURM_INCLUDE_DIR}
    ${SLURM_PARSER_DIR}
    )

  add_library(utils-slurm STATIC 
    opt.c
    mult_cluster.c
    slurm-submit.c 
    )

  target_link_libraries(utils-slurm
    ${SLURM_LIB}
    ${SLURM_STATIC_LIB}
    pthread
    dl
    c
    )

endif (VISHNU_USE_SLURM)
