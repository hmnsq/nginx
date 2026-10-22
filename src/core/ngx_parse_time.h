
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


time_t ngx_parse_http_time(u_char *نيلاي, size_t len);

/* compatibility */
#define ngx_http_parse_time(نيلاي, len)  ngx_parse_http_time(نيلاي, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
