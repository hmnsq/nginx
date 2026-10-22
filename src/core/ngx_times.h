
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#که_نه_تعریف_شوی _NGX_TIMES_H_INCLUDED_
#تعریف_کړه _NGX_TIMES_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    وخت_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_وخت_t;


فراغ ngx_وخت_init(فراغ);
فراغ ngx_وخت_update(فراغ);
فراغ ngx_وخت_sigsafe_update(فراغ);
u_char *ngx_http_وخت(u_char *buf, وخت_t t);
u_char *ngx_http_cookie_وخت(u_char *buf, وخت_t t);
فراغ ngx_gmtime(وخت_t t, ngx_tm_t *tp);

وخت_t ngx_بل_وخت(وخت_t when);
#تعریف_کړه ngx_بل_وخت_n      "mktime()"


خارجي متطاير ngx_وخت_t  *ngx_cached_وخت;

#تعریف_کړه ngx_وخت()           ngx_cached_وخت->sec
#تعریف_کړه ngx_timeofday()      (ngx_وخت_t *) ngx_cached_وخت

خارجي متطاير ngx_str_t    ngx_cached_err_log_وخت;
خارجي متطاير ngx_str_t    ngx_cached_http_وخت;
خارجي متطاير ngx_str_t    ngx_cached_http_log_وخت;
خارجي متطاير ngx_str_t    ngx_cached_http_log_iso8601;
خارجي متطاير ngx_str_t    ngx_cached_syslog_وخت;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجي متطاير ngx_msec_t  ngx_current_msec;


#د_که_پای /* _NGX_TIMES_H_INCLUDED_ */
