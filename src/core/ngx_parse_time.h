
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PARSE_TIME_H_INCLUDED_
#define _NGX_PARSE_TIME_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


समय_प्रकार ngx_parse_http_time(u_char *मान, माप_प्रकार len);

/* compatibility */
#define ngx_http_parse_time(मान, len)  ngx_parse_http_time(मान, len)


#endif /* _NGX_PARSE_TIME_H_INCLUDED_ */
