
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_CRYPT_H_INCLUDED_
#నిర్వచించు _NGX_CRYPT_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


ngx_int_t ngx_crypt(ngx_pool_t *pool, u_char *కీలకం, u_char *salt,
    u_char **encrypted);


#అయితే_ముగింపు /* _NGX_CRYPT_H_INCLUDED_ */
