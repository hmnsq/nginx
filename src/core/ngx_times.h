
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIMES_H_INCLUDED_
#define _NGX_TIMES_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


ಶೂನ್ಯ ngx_time_init(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_time_update(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_time_sigsafe_update(ಶೂನ್ಯ);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
ಶೂನ್ಯ ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n      "mktime()"


ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_time_t  *ngx_cached_time;

#define ngx_time()           ngx_cached_time->sec
#define ngx_timeofday()      (ngx_time_t *) ngx_cached_time

ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_str_t    ngx_cached_err_log_time;
ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_str_t    ngx_cached_http_time;
ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_str_t    ngx_cached_http_log_time;
ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_str_t    ngx_cached_http_log_iso8601;
ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
ಬಾಹ್ಯ ಚಞ್ಚಲ ngx_msec_t  ngx_current_msec;


#endif /* _NGX_TIMES_H_INCLUDED_ */
