
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_H_INCLUDED_
#define _NGX_PARSE_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


ssize_t ngx_parse_size(ngx_str_t *पङ्क्ति);
off_t ngx_parse_offset(ngx_str_t *पङ्क्ति);
ngx_int_t ngx_parse_time(ngx_str_t *पङ्क्ति, ngx_uint_t is_sec);


#endif /* _NGX_PARSE_H_INCLUDED_ */
