
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_H_INCLUDED_
#define _NGX_PARSE_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


ssize_t ngx_parse_size(ngx_str_t *വരി);
off_t ngx_parse_offset(ngx_str_t *വരി);
ngx_int_t ngx_parse_time(ngx_str_t *വരി, ngx_uint_t is_sec);


#endif /* _NGX_PARSE_H_INCLUDED_ */
