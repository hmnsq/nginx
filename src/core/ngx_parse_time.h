
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే NGX_PARSE_సమయం_H_INCLUDED
#నిర్వచించు NGX_PARSE_సమయం_H_INCLUDED


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


సమయం_t ngx_parse_http_సమయం(u_char *విలువ, కొలత_t len);

/* compatibility */
#నిర్వచించు ngx_http_parse_సమయం(విలువ, len)  ngx_parse_http_సమయం(విలువ, len)


#అయితే_ముగింపు /* _NGX_PARSE_TIME_H_INCLUDED_ */
