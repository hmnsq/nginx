
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#నిర్వచితం_కాకపోతే _NGX_TIMES_H_INCLUDED_
#నిర్వచించు _NGX_TIMES_H_INCLUDED_


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


రకనిర్వచనం నిర్మాణం {
    సమయం_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_సమయం_t;


శూన్య ngx_సమయం_init(శూన్య);
శూన్య ngx_సమయం_update(శూన్య);
శూన్య ngx_సమయం_sigsafe_update(శూన్య);
u_char *ngx_http_సమయం(u_char *buf, సమయం_t t);
u_char *ngx_http_cookie_సమయం(u_char *buf, సమయం_t t);
శూన్య ngx_gmtime(సమయం_t t, ngx_tm_t *tp);

సమయం_t ngx_తదుపరి_సమయం(సమయం_t when);
#నిర్వచించు ngx_తదుపరి_సమయం_n      "mktime()"


బాహ్య అస్థిర ngx_సమయం_t  *ngx_cached_సమయం;

#నిర్వచించు ngx_సమయం()           ngx_cached_సమయం->sec
#నిర్వచించు ngx_timeofday()      (ngx_సమయం_t *) ngx_cached_సమయం

బాహ్య అస్థిర ngx_str_t    ngx_cached_err_log_సమయం;
బాహ్య అస్థిర ngx_str_t    ngx_cached_http_సమయం;
బాహ్య అస్థిర ngx_str_t    ngx_cached_http_log_సమయం;
బాహ్య అస్థిర ngx_str_t    ngx_cached_http_log_iso8601;
బాహ్య అస్థిర ngx_str_t    ngx_cached_syslog_సమయం;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
బాహ్య అస్థిర ngx_msec_t  ngx_current_msec;


#అయితే_ముగింపు /* _NGX_TIMES_H_INCLUDED_ */
