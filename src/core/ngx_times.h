
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIMES_H_INCLUDED_
#define _NGX_TIMES_H_INCLUDED_


#ଅନ୍ତର୍ଭାଵ <ngx_config.h>
#ଅନ୍ତର୍ଭାଵ <ngx_core.h>


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


ଶୂନ୍ୟ ngx_time_init(ଶୂନ୍ୟ);
ଶୂନ୍ୟ ngx_time_update(ଶୂନ୍ୟ);
ଶୂନ୍ୟ ngx_time_sigsafe_update(ଶୂନ୍ୟ);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
ଶୂନ୍ୟ ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n      "mktime()"


ବାହ୍ୟ ଅସ୍ଥିର ngx_time_t  *ngx_cached_time;

#define ngx_time()           ngx_cached_time->sec
#define ngx_timeofday()      (ngx_time_t *) ngx_cached_time

ବାହ୍ୟ ଅସ୍ଥିର ngx_str_t    ngx_cached_err_log_time;
ବାହ୍ୟ ଅସ୍ଥିର ngx_str_t    ngx_cached_http_time;
ବାହ୍ୟ ଅସ୍ଥିର ngx_str_t    ngx_cached_http_log_time;
ବାହ୍ୟ ଅସ୍ଥିର ngx_str_t    ngx_cached_http_log_iso8601;
ବାହ୍ୟ ଅସ୍ଥିର ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
ବାହ୍ୟ ଅସ୍ଥିର ngx_msec_t  ngx_current_msec;


#endif /* _NGX_TIMES_H_INCLUDED_ */
