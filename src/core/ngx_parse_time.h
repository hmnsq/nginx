
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


time_t ngx_parse_http_time(u_char *કિંમત, size_t len);

/* compatibility */
#define ngx_http_parse_time(કિંમત, len)  ngx_parse_http_time(કિંમત, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
