
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


time_t ngx_parse_http_time(u_char *মান, আকার_t len);

/* compatibility */
#define ngx_http_parse_time(মান, len)  ngx_parse_http_time(মান, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
