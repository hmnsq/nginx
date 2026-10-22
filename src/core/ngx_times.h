
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _NGX_TIMES_H_INCLUDED_
#ವ್ಯಾಖ್ಯಾನಿಸು _NGX_TIMES_H_INCLUDED_


#ಅನ್ತರ್ಭಾವ <ngx_config.h>
#ಅನ್ತರ್ಭಾವ <ngx_core.h>


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸಮಯ_t      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_ಸಮಯ_t;


ಶೂನ್ಯ ngx_ಸಮಯ_init(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_ಸಮಯ_update(ಶೂನ್ಯ);
ಶೂನ್ಯ ngx_ಸಮಯ_sigsafe_update(ಶೂನ್ಯ);
u_char *ngx_http_ಸಮಯ(u_char *buf, ಸಮಯ_t t);
u_char *ngx_http_cookie_ಸಮಯ(u_char *buf, ಸಮಯ_t t);
ಶೂನ್ಯ ngx_gmtime(ಸಮಯ_t t, ngx_tm_t *tp);

ಸಮಯ_t ngx_ಮುಂದಿನ_ಸಮಯ(ಸಮಯ_t when);
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ಮುಂದಿನ_ಸಮಯ_n      "mktime()"


ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_ಸಮಯ_t  *ngx_cached_ಸಮಯ;

#ವ್ಯಾಖ್ಯಾನಿಸು ngx_ಸಮಯ()           ngx_cached_ಸಮಯ->sec
#ವ್ಯಾಖ್ಯಾನಿಸು ngx_timeofday()      (ngx_ಸಮಯ_t *) ngx_cached_ಸಮಯ

ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_str_t    ngx_cached_err_log_ಸಮಯ;
ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_str_t    ngx_cached_http_ಸಮಯ;
ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_str_t    ngx_cached_http_log_ಸಮಯ;
ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_str_t    ngx_cached_http_log_iso8601;
ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_str_t    ngx_cached_syslog_ಸಮಯ;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
ಬಾಹ್ಯ ಅಸ್ಥಿರ ngx_msec_t  ngx_current_msec;


#ಆದರೆ_ಅಂತ್ಯ /* _NGX_TIMES_H_INCLUDED_ */
