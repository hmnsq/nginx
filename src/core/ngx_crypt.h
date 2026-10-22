
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CRYPT_H_INCLUDED_
#define _NGX_CRYPT_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_int_t ngx_crypt(ngx_pool_t *pool, u_char *مبودي, u_char *salt,
    u_char **encrypted);


#endif /* _NGX_CRYPT_H_INCLUDED_ */
