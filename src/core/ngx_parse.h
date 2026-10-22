
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_PARSE_H_INCLUDED_
#تعریف_کرو _NGX_PARSE_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


ssize_t ngx_parse_سائز(ngx_str_t *سطر);
off_t ngx_parse_انحراف(ngx_str_t *سطر);
ngx_int_t ngx_parse_وقت(ngx_str_t *سطر, ngx_uint_t is_sec);


#اگر_ختم /* _NGX_PARSE_H_INCLUDED_ */
