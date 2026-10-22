
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#यदि_अपरिभाषितम् _NGX_PARSE_TIME_H_INCLUDED_
#परिभाषय _NGX_PARSE_TIME_H_INCLUDED_


#अन्तर्भाव <ngx_config.h>
#अन्तर्भाव <ngx_core.h>


time_t ngx_parse_http_time(u_char *मानम्, आकार_t len);

/* compatibility */
#परिभाषय ngx_http_parse_time(मानम्, len)  ngx_parse_http_time(मानम्, len)


#यदि_अन्तः /* _NGX_PARSE_TIME_H_INCLUDED_ */
