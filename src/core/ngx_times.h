
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جيڪڏهن_وصف_نه_ٿيل _NGX_TIMES_H_INCLUDED_
#وصف_ڪريو _NGX_TIMES_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    وقت_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_وقت_t;


فراغ ngx_وقت_init(فراغ);
فراغ ngx_وقت_update(فراغ);
فراغ ngx_وقت_sigsafe_update(فراغ);
u_char *ngx_http_وقت(u_char *buf, وقت_t t);
u_char *ngx_http_cookie_وقت(u_char *buf, وقت_t t);
فراغ ngx_gmtime(وقت_t t, ngx_tm_t *tp);

وقت_t ngx_ايندڙ_وقت(وقت_t when);
#وصف_ڪريو ngx_ايندڙ_وقت_n      "mktime()"


خارجي متطاير ngx_وقت_t  *ngx_cached_وقت;

#وصف_ڪريو ngx_وقت()           ngx_cached_وقت->sec
#وصف_ڪريو ngx_timeofday()      (ngx_وقت_t *) ngx_cached_وقت

خارجي متطاير ngx_str_t    ngx_cached_err_log_وقت;
خارجي متطاير ngx_str_t    ngx_cached_http_وقت;
خارجي متطاير ngx_str_t    ngx_cached_http_log_وقت;
خارجي متطاير ngx_str_t    ngx_cached_http_log_iso8601;
خارجي متطاير ngx_str_t    ngx_cached_syslog_وقت;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجي متطاير ngx_msec_t  ngx_current_msec;


#جيڪڏهن_پورو /* _NGX_TIMES_H_INCLUDED_ */
