
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


time_t ngx_parse_http_time(u_char *ମୂଲ୍ୟ, size_t len);

/* compatibility */
#define ngx_http_parse_time(ମୂଲ୍ୟ, len)  ngx_parse_http_time(ମୂଲ୍ୟ, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
