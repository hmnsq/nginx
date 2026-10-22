
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_TIMES_H_INCLUDED_
#define _NGX_TIMES_H_INCLUDED_


#અન્તર્ભાવ <ngx_config.h>
#અન્તર્ભાવ <ngx_core.h>


પ્રકારવ્યાખ્યા સંરચના {
    time_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


શૂન્ય ngx_time_init(શૂન્ય);
શૂન્ય ngx_time_update(શૂન્ય);
શૂન્ય ngx_time_sigsafe_update(શૂન્ય);
u_char *ngx_http_time(u_char *buf, time_t t);
u_char *ngx_http_cookie_time(u_char *buf, time_t t);
શૂન્ય ngx_gmtime(time_t t, ngx_tm_t *tp);

time_t ngx_next_time(time_t when);
#define ngx_next_time_n      "mktime()"


બાહ્ય અસ્થિર ngx_time_t  *ngx_cached_time;

#define ngx_time()           ngx_cached_time->sec
#define ngx_timeofday()      (ngx_time_t *) ngx_cached_time

બાહ્ય અસ્થિર ngx_str_t    ngx_cached_err_log_time;
બાહ્ય અસ્થિર ngx_str_t    ngx_cached_http_time;
બાહ્ય અસ્થિર ngx_str_t    ngx_cached_http_log_time;
બાહ્ય અસ્થિર ngx_str_t    ngx_cached_http_log_iso8601;
બાહ્ય અસ્થિર ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
બાહ્ય અસ્થિર ngx_msec_t  ngx_current_msec;


#endif /* _NGX_TIMES_H_INCLUDED_ */
