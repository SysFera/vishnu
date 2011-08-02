
#ifndef _HAVE_IO_HDR_H
#define _HAVE_IO_HDR_H


#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#if HAVE_STDINT_H
#  include <stdint.h>
#endif
#if HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "macros.h"   /* Containes SLURM_CRED_SIGLEN */
#include "pack.h"
#include "cbuf.h"
#include "xmalloc.h"

#define MAX_MSG_LEN 1024
#define SLURM_IO_KEY_SIZE 8

#define SLURM_IO_STDIN 0
#define SLURM_IO_STDOUT 1
#define SLURM_IO_STDERR 2
#define SLURM_IO_ALLSTDIN 3
#define SLURM_IO_CONNECTION_TEST 4

struct slurm_io_init_msg {
	uint16_t      version;
	unsigned char cred_signature[SLURM_IO_KEY_SIZE];
	uint32_t      nodeid;
	uint32_t      stdout_objs;
	uint32_t      stderr_objs;
};


typedef struct slurm_io_header {
	uint16_t      type;
	uint16_t      gtaskid;
	uint16_t      ltaskid;
	uint32_t      length;
} io_hdr_t;

/*
 * Return the packed size of an IO header in bytes;
 */
int io_hdr_packed_size();
void io_hdr_pack(io_hdr_t *hdr, Buf buffer);
int io_hdr_unpack(io_hdr_t *hdr, Buf buffer);
int io_hdr_read_fd(int fd, io_hdr_t *hdr);

/*
 * Validate io init msg
 */
int io_init_msg_validate(struct slurm_io_init_msg *msg, const char *sig);
int io_init_msg_write_to_fd(int fd, struct slurm_io_init_msg *msg);
int io_init_msg_read_from_fd(int fd, struct slurm_io_init_msg *msg);

#endif /* !_HAVE_IO_HDR_H */
