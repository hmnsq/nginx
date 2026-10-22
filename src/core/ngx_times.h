
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIMES_H_INCLUDED_
#define _NGX_TIMES_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


فراغ ngx_time_init(فراغ);
فراغ ngx_time_update(فراغ);
فراغ ngx_time_sigsafe_update(فراغ);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
فراغ ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n      "mktime()"


خارجي متطاير ngx_time_t  *ngx_cached_time;

#define ngx_time()           ngx_cached_time->sec
#define ngx_timeofday()      (ngx_time_t *) ngx_cached_time

خارجي متطاير ngx_str_t    ngx_cached_err_log_time;
خارجي متطاير ngx_str_t    ngx_cached_http_time;
خارجي متطاير ngx_str_t    ngx_cached_http_log_time;
خارجي متطاير ngx_str_t    ngx_cached_http_log_iso8601;
خارجي متطاير ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجي متطاير ngx_msec_t  ngx_current_msec;


#endif /* _NGX_TIMES_H_INCLUDED_ */
