/*
 * C
 *
 * Copyright 2014-2016 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLNET_ERRORS.h"
#include <lwip/err.h>

int32_t map_to_java_exception(int32_t err){
	switch(err){
		case (EACCES):
			return J_EACCES;
		case (EBADF):
			return J_EBADF;
		case (EHOSTDOWN):
			return J_EHOSTDOWN;
		case (ENETDOWN):
			return J_ENETDOWN;
		case (ENETUNREACH):
			return J_ENETUNREACH;
		case (EADDRINUSE):
			return J_EADDRINUSE;
		case (EINVAL):
			return J_EINVAL;
		case (ECONNABORTED):
			return J_ECONNABORTED;
		case (ENOPROTOOPT):
			return J_ENOPROTOOPT;
		case (ENOTCONN):
			return J_ENOTCONN;
		case (EAFNOSUPPORT):
			return J_EAFNOSUPPORT;
		case (ECONNREFUSED):
			return J_ECONNREFUSED;
		case (EISCONN):
			return J_EISCONN;
		case (ECONNRESET):
			return J_ECONNRESET;
		case (EMSGSIZE):
			return J_EMSGSIZE;
		case (EPIPE):
			return J_EPIPE;
		case (ETIMEDOUT):
			return J_ETIMEDOUT;
		case (ENOBUFS):
		case (ENOSPC):
		case (ENOMEM):
			return J_ENOMEM;
		case (EHOSTUNREACH):
			return J_EHOSTUNREACH;
		default:
			return J_EUNKNOWN;
	}
}
