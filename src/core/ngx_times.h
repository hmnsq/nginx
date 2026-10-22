
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#جے_بنا_تعریف _NGX_TIMES_H_INCLUDED_
#تعریف_کرو _NGX_TIMES_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    ویلا_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_ویلا_t;


فراغ ngx_ویلا_init(فراغ);
فراغ ngx_ویلا_update(فراغ);
فراغ ngx_ویلا_sigsafe_update(فراغ);
u_char *ngx_http_ویلا(u_char *buf, ویلا_t t);
u_char *ngx_http_cookie_ویلا(u_char *buf, ویلا_t t);
فراغ ngx_gmtime(ویلا_t t, ngx_tm_t *tp);

ویلا_t ngx_اگلا_ویلا(ویلا_t when);
#تعریف_کرو ngx_اگلا_ویلا_n      "mktime()"


خارجي متطاير ngx_ویلا_t  *ngx_cached_ویلا;

#تعریف_کرو ngx_ویلا()           ngx_cached_ویلا->sec
#تعریف_کرو ngx_timeofday()      (ngx_ویلا_t *) ngx_cached_ویلا

خارجي متطاير ngx_str_t    ngx_cached_err_log_ویلا;
خارجي متطاير ngx_str_t    ngx_cached_http_ویلا;
خارجي متطاير ngx_str_t    ngx_cached_http_log_ویلا;
خارجي متطاير ngx_str_t    ngx_cached_http_log_iso8601;
خارجي متطاير ngx_str_t    ngx_cached_syslog_ویلا;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجي متطاير ngx_msec_t  ngx_current_msec;


#جے_انت /* _NGX_TIMES_H_INCLUDED_ */
