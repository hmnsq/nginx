
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIMES_H_INCLUDED_
#define _NGX_TIMES_H_INCLUDED_


#অন্তর্ভাব <ngx_config.h>
#অন্তর্ভাব <ngx_core.h>


প্রকারনির্দেশ সংরচনা {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


শূন্য ngx_time_init(শূন্য);
শূন্য ngx_time_update(শূন্য);
শূন্য ngx_time_sigsafe_update(শূন্য);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
শূন্য ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n      "mktime()"


বাহ্য চঞ্চল ngx_time_t  *ngx_cached_time;

#define ngx_time()           ngx_cached_time->sec
#define ngx_timeofday()      (ngx_time_t *) ngx_cached_time

বাহ্য চঞ্চল ngx_str_t    ngx_cached_err_log_time;
বাহ্য চঞ্চল ngx_str_t    ngx_cached_http_time;
বাহ্য চঞ্চল ngx_str_t    ngx_cached_http_log_time;
বাহ্য চঞ্চল ngx_str_t    ngx_cached_http_log_iso8601;
বাহ্য চঞ্চল ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
বাহ্য চঞ্চল ngx_msec_t  ngx_current_msec;


#endif /* _NGX_TIMES_H_INCLUDED_ */
