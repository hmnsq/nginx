
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#अगर_अघोषित _NGX_TIMES_H_INCLUDED_
#घोषणा _NGX_TIMES_H_INCLUDED_


#समावेश <ngx_config.h>
#समावेश <ngx_core.h>


प्रकार काष्ठा {
    समय_प्रकार      sec;
    ngx_uint_t  msec;
    ngx_int_t   gmtoff;
} ngx_time_t;


व्योम ngx_time_init(व्योम);
व्योम ngx_time_update(व्योम);
व्योम ngx_time_sigsafe_update(व्योम);
u_char *ngx_http_time(u_char *buf, समय_प्रकार t);
u_char *ngx_http_cookie_time(u_char *buf, समय_प्रकार t);
व्योम ngx_gmtime(समय_प्रकार t, ngx_tm_t *tp);

समय_प्रकार ngx_अग्र_time(समय_प्रकार when);
#घोषणा ngx_अग्र_time_n      "mktime()"


बाह्य अस्थिर ngx_time_t  *ngx_cached_time;

#घोषणा ngx_time()           ngx_cached_time->sec
#घोषणा ngx_timeofday()      (ngx_time_t *) ngx_cached_time

बाह्य अस्थिर ngx_str_t    ngx_cached_err_log_time;
बाह्य अस्थिर ngx_str_t    ngx_cached_http_time;
बाह्य अस्थिर ngx_str_t    ngx_cached_http_log_time;
बाह्य अस्थिर ngx_str_t    ngx_cached_http_log_iso8601;
बाह्य अस्थिर ngx_str_t    ngx_cached_syslog_time;

/*
 * milliseconds elapsed since some unspecified point in the past
 * and truncated to ngx_msec_t, used in event timers
 */
बाह्य अस्थिर ngx_msec_t  ngx_current_msec;


#पूर्ण_अगर /* _NGX_TIMES_H_INCLUDED_ */
