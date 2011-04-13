# setup in-place testing
file( GLOB CFG_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cfg.in" )
foreach( INPUT ${CFG_SRCS} )
  string( REGEX REPLACE "(.*).in$" "\\1" OUTPUT ${INPUT} )
  configure_file( ${INPUT} ${CONFIG_DIR}/${OUTPUT} @ONLY )
endforeach()

file( GLOB CFG_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cfg" )
file( COPY ${CFG_SRCS} DESTINATION ${CONFIG_DIR} )


 

