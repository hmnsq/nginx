
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده NGX_PARSE_زمان_H_INCLUDED
#تعریف_کن NGX_PARSE_زمان_H_INCLUDED


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


زمان_t ngx_parse_http_زمان(u_char *مقدار, اندازه_t len);

/* compatibility */
#تعریف_کن ngx_http_parse_زمان(مقدار, len)  ngx_parse_http_زمان(مقدار, len)


#پایان_اگر /* _NGX_PARSE_TIME_H_INCLUDED_ */
