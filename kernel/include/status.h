#ifndef _KERNEL_STATUS_H
#define _KERNEL_STATUS_H

#include <types.h>

typedef enum _kstatus
{
	STATUS_SUCCESS = 0x00000000,
	STATUS_ERROR = 0x80000000,
	STATUS_ASSERTION_FAILED,
	STATUS_PORT_UNAVAILABLE
} KSTATUS;

#define KSUCCESS(status) (((status) >= STATUS_SUCCESS) ? 1 : 0)
#define KERROR(status) (((status) < STATUS_SUCCESS) ? 1 : 0)

#endif