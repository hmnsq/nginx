
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#அந்தர்பாவ <ngx_config.h>
#அந்தர்பாவ <ngx_core.h>


time_t ngx_parse_http_time(u_char *மதிப்பு, size_t len);

/* compatibility */
#define ngx_http_parse_time(மதிப்பு, len)  ngx_parse_http_time(மதிப்பு, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
