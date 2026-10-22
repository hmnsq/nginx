
/*
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SIPHASH_H_INCLUDED_
#define _NGX_SIPHASH_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


uint64_t ngx_siphash(uint64_t k0, uint64_t k1, u_char *data, size_t len);


#endif /* _NGX_SIPHASH_H_INCLUDED_ */
