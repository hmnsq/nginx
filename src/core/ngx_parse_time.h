
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


time_t ngx_parse_http_time(u_char *ಮೌಲ್ಯ, size_t len);

/* compatibility */
#define ngx_http_parse_time(ಮೌಲ್ಯ, len)  ngx_parse_http_time(ಮೌಲ್ಯ, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
