
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


time_t ngx_parse_http_time(u_char *مقدار, size_t len);

/* compatibility */
#define ngx_http_parse_time(مقدار, len)  ngx_parse_http_time(مقدار, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
