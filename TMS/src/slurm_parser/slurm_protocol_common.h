
#ifndef _SLURM_PROTOCOL_COMMON_H
#define _SLURM_PROTOCOL_COMMON_H

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <sys/time.h>
#include <time.h>

#include <slurm/slurm_errno.h>

/* for sendto and recvfrom commands */
#define SLURM_PROTOCOL_NO_SEND_RECV_FLAGS 0

/* for listen API */
#ifndef SLURM_PROTOCOL_DEFAULT_LISTEN_BACKLOG
#define SLURM_PROTOCOL_DEFAULT_LISTEN_BACKLOG 4096
#endif

/* used in interface methods */
#define SLURM_PROTOCOL_FUNCTION_NOT_IMPLEMENTED -2

/* max slurm message send and receive buffer size
 * this may need to be increased to 350k-512k */
#define SLURM_PROTOCOL_MAX_MESSAGE_BUFFER_SIZE (512*1024)

/* slurm protocol header defines, based upon config.h, 16 bits */
/* A new SLURM_PROTOCOL_VERSION needs to be made each time the version
 * changes so the slurmdbd can talk all versions for update messages.
 * In slurm_protocol_util.c check_header_version(), and init_header()
 * need to be updated also when changes are added */
#define SLURM_PROTOCOL_VERSION ((SLURM_API_MAJOR << 8) | SLURM_API_AGE)
#define SLURM_2_2_PROTOCOL_VERSION SLURM_PROTOCOL_VERSION
#define SLURM_2_1_PROTOCOL_VERSION ((21 << 8) | 0)
#define SLURM_2_0_PROTOCOL_VERSION ((20 << 8) | 0)
#define SLURM_1_3_PROTOCOL_VERSION ((13 << 8) | 0)

/* used to set flags to empty */
#define SLURM_PROTOCOL_NO_FLAGS 0
#define SLURM_GLOBAL_AUTH_KEY   0x0001

#if MONGO_IMPLEMENTATION
#  include <slurm_protocol_mongo_common.h>
#else
#  include <slurm_protocol_socket_common.h>
#endif

#endif
