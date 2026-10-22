
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_PARSE_H_INCLUDED_
#تعریف_کن _NGX_PARSE_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


ssize_t ngx_parse_اندازه(ngx_str_t *سطر);
off_t ngx_parse_جابه_جایی(ngx_str_t *سطر);
ngx_int_t ngx_parse_زمان(ngx_str_t *سطر, ngx_uint_t is_sec);


#پایان_اگر /* _NGX_PARSE_H_INCLUDED_ */
