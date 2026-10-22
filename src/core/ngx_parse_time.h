
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی NGX_PARSE_وخت_H_INCLUDED
#تعریف_کړه NGX_PARSE_وخت_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


وخت_t ngx_parse_http_وخت(u_char *ارزښت, اندازه_t len);

/* compatibility */
#تعریف_کړه ngx_http_parse_وخت(ارزښت, len)  ngx_parse_http_وخت(ارزښت, len)


#د_که_پای /* _NGX_PARSE_TIME_H_INCLUDED_ */
