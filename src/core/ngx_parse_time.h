
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


time_t ngx_parse_http_time(u_char *قدر, size_t len);

/* compatibility */
#define ngx_http_parse_time(قدر, len)  ngx_parse_http_time(قدر, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
