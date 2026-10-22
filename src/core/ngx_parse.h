
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_H_INCLUDED_
#define _NGX_PARSE_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ssize_t ngx_parse_size(ngx_str_t *ଧାଡ଼ି);
off_t ngx_parse_offset(ngx_str_t *ଧାଡ଼ି);
ngx_int_t ngx_parse_time(ngx_str_t *ଧାଡ଼ି, ngx_uint_t is_sec);


#endif /* _NGX_PARSE_H_INCLUDED_ */
