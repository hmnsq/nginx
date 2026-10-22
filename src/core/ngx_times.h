
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _NGX_TIMES_H_INCLUDED_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _NGX_TIMES_H_INCLUDED_


#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ngx_core.h>


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਸਮਾਂ_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_ਸਮਾਂ_t;


ਖਾਲੀ ngx_ਸਮਾਂ_init(ਖਾਲੀ);
ਖਾਲੀ ngx_ਸਮਾਂ_update(ਖਾਲੀ);
ਖਾਲੀ ngx_ਸਮਾਂ_sigsafe_update(ਖਾਲੀ);
u_char *ngx_http_ਸਮਾਂ(u_char *buf, ਸਮਾਂ_t t);
u_char *ngx_http_cookie_ਸਮਾਂ(u_char *buf, ਸਮਾਂ_t t);
ਖਾਲੀ ngx_gmtime(ਸਮਾਂ_t t, ngx_tm_t *tp);

ਸਮਾਂ_t ngx_ਅਗਲਾ_ਸਮਾਂ(ਸਮਾਂ_t when);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਅਗਲਾ_ਸਮਾਂ_n      "mktime()"


ਬਾਹਰੀ ਅਸਥਿਰ ngx_ਸਮਾਂ_t  *ngx_cached_ਸਮਾਂ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_ਸਮਾਂ()           ngx_cached_ਸਮਾਂ->sec
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ngx_timeofday()      (ngx_ਸਮਾਂ_t *) ngx_cached_ਸਮਾਂ

ਬਾਹਰੀ ਅਸਥਿਰ ngx_str_t    ngx_cached_err_log_ਸਮਾਂ;
ਬਾਹਰੀ ਅਸਥਿਰ ngx_str_t    ngx_cached_http_ਸਮਾਂ;
ਬਾਹਰੀ ਅਸਥਿਰ ngx_str_t    ngx_cached_http_log_ਸਮਾਂ;
ਬਾਹਰੀ ਅਸਥਿਰ ngx_str_t    ngx_cached_http_log_iso8601;
ਬਾਹਰੀ ਅਸਥਿਰ ngx_str_t    ngx_cached_syslog_ਸਮਾਂ;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
ਬਾਹਰੀ ਅਸਥਿਰ ngx_msec_t  ngx_current_msec;


#ਜੇ_ਅੰਤ /* _NGX_TIMES_H_INCLUDED_ */
