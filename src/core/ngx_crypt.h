
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_CRYPT_H_INCLUDED_
#تعریف_کرو _NGX_CRYPT_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ngx_int_t ngx_crypt(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);


#اگر_ختم /* _NGX_CRYPT_H_INCLUDED_ */
