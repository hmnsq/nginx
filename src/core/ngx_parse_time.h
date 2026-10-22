
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#അന്തര്ഭാവ <ngx_config.h>
#അന്തര്ഭാവ <ngx_core.h>


time_t ngx_parse_http_time(u_char *മൂല്യം, size_t len);

/* compatibility */
#define ngx_http_parse_time(മൂല്യം, len)  ngx_parse_http_time(മൂല്യം, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
