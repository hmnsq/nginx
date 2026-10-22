
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#اگر_تعریف_نشده _NGX_TIMES_H_INCLUDED_
#تعریف_کن _NGX_TIMES_H_INCLUDED_


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>


تعريفنوع بنية {
    زمان_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_زمان_t;


فراغ ngx_زمان_init(فراغ);
فراغ ngx_زمان_update(فراغ);
فراغ ngx_زمان_sigsafe_update(فراغ);
u_char *ngx_http_زمان(u_char *buf, زمان_t t);
u_char *ngx_http_cookie_زمان(u_char *buf, زمان_t t);
فراغ ngx_gmtime(زمان_t t, ngx_tm_t *tp);

زمان_t ngx_بعدی_زمان(زمان_t when);
#تعریف_کن ngx_بعدی_زمان_n      "mktime()"


خارجي متطاير ngx_زمان_t  *ngx_cached_زمان;

#تعریف_کن ngx_زمان()           ngx_cached_زمان->sec
#تعریف_کن ngx_timeofday()      (ngx_زمان_t *) ngx_cached_زمان

خارجي متطاير ngx_str_t    ngx_cached_err_log_زمان;
خارجي متطاير ngx_str_t    ngx_cached_http_زمان;
خارجي متطاير ngx_str_t    ngx_cached_http_log_زمان;
خارجي متطاير ngx_str_t    ngx_cached_http_log_iso8601;
خارجي متطاير ngx_str_t    ngx_cached_syslog_زمان;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
خارجي متطاير ngx_msec_t  ngx_current_msec;


#پایان_اگر /* _NGX_TIMES_H_INCLUDED_ */
