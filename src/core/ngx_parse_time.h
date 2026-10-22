
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ NGX_PARSE_وقت_H_INCLUDED
#تعریف_کرو NGX_PARSE_وقت_H_INCLUDED


#شامل <ngx_config.h>
#شامل <ngx_core.h>


وقت_t ngx_parse_http_وقت(u_char *قدر, سائز_t len);

/* compatibility */
#تعریف_کرو ngx_http_parse_وقت(قدر, len)  ngx_parse_http_وقت(قدر, len)


#اگر_ختم /* _NGX_PARSE_TIME_H_INCLUDED_ */
