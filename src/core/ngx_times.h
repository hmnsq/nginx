
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_غیر_تعریف_شدہ _NGX_TIMES_H_INCLUDED_
#تعریف_کرو _NGX_TIMES_H_INCLUDED_


#شامل <ngx_config.h>
#شامل <ngx_core.h>


قسمتعریف ساخت {
    وقت_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_وقت_t;


خالی ngx_وقت_init(خالی);
خالی ngx_وقت_update(خالی);
خالی ngx_وقت_sigsafe_update(خالی);
u_char *ngx_http_وقت(u_char *buf, وقت_t t);
u_char *ngx_http_cookie_وقت(u_char *buf, وقت_t t);
خالی ngx_gmtime(وقت_t t, ngx_tm_t *tp);

وقت_t ngx_اگلا_وقت(وقت_t when);
#تعریف_کرو ngx_اگلا_وقت_n      "mktime()"


خارجی متلون ngx_وقت_t  *ngx_cached_وقت;

#تعریف_کرو ngx_وقت()           ngx_cached_وقت->sec
#تعریف_کرو ngx_timeofday()      (ngx_وقت_t *) ngx_cached_وقت

خارجی متلون ngx_str_t    ngx_cached_err_log_وقت;
خارجی متلون ngx_str_t    ngx_cached_http_وقت;
خارجی متلون ngx_str_t    ngx_cached_http_log_وقت;
خارجی متلون ngx_str_t    ngx_cached_http_log_iso8601;
خارجی متلون ngx_str_t    ngx_cached_syslog_وقت;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجی متلون ngx_msec_t  ngx_current_msec;


#اگر_ختم /* _NGX_TIMES_H_INCLUDED_ */
