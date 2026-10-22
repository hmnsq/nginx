
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_CRYPT_H_INCLUDED_
#تعریف_کن _NGX_CRYPT_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_int_t ngx_crypt(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);


#پایان_اگر /* _NGX_CRYPT_H_INCLUDED_ */
