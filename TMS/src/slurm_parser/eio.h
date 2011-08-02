
#ifndef _EIO_H
#define _EIO_H 1

//#include "list.h"
#include <slurm.h>
#include "macros.h"
#include "slurm_protocol_defs.h"
typedef struct eio_obj eio_obj_t;

typedef struct eio_handle_components eio_handle_t;


struct eio_obj {
	int fd;                           /* fd to operate on                */
	void *arg;                        /* application-specific data       */
	struct io_operations *ops;        /* pointer to ops struct for obj   */
	bool shutdown;
};


#endif /* !_EIO_H */
