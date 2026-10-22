
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


time_t ngx_parse_http_time(u_char *मूल्य, size_t len);

/* compatibility */
#define ngx_http_parse_time(मूल्य, len)  ngx_parse_http_time(मूल्य, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
