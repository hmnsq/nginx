
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_CRYPT_H_INCLUDED_
#تعریف_کرو _NGX_CRYPT_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ngx_int_t ngx_crypt(ngx_pool_t *pool, u_char *کلید, u_char *salt,
    u_char **encrypted);


#جے_انت /* _NGX_CRYPT_H_INCLUDED_ */
