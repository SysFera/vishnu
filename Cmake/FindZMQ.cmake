#
# Try to find the SLURM installation
#

find_path(ZMQ_INCLUDE_DIR
  zmq.hpp
  paths
	${ZMQ_DIR}/include
	$ENV{ZMQ_DIR}/include
	/usr/include
	/usr/local/include
	/opt/local/include
)


find_library(ZMQ_LIB
  zmq
  paths
	${ZMQ_DIR}/lib
  $ENV{ZMQ_DIR}/lib
	/usr/lib
	/usr/local/lib
  /opt/local/lib
)

if (ZMQ_INCLUDE_DIR AND  ZMQ_LIB)
  set(ZMQ_FOUND "Yes")
  mark_as_advanced(ZMQ_INCLUDE_DIR)
  mark_as_advanced(ZMQ_LIB)
endif(ZMQ_INCLUDE_DIR AND ZMQ_LIB)

