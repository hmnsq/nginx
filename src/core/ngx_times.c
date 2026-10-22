
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>


స్థిర ngx_msec_t ngx_monotonic_సమయం(సమయం_t sec, ngx_uint_t msec);


/*
 * The time may be updated by signal handler or by several threads.
 * The time update operations are rare and require to hold the ngx_time_lock.
 * The time read operations are frequent, so they are lock-free and get time
 * values and strings from the current slot.  Thus thread may get the corrupted
 * values only if it is preempted while copying and then it is not scheduled
 * to run more than NGX_TIME_SLOTS seconds.
 */

#నిర్వచించు NGX_సమయం_SLOTS   64

స్థిర ngx_uint_t        స్థానం;
స్థిర ngx_atomic_t      ngx_సమయం_తాళం;

అస్థిర ngx_msec_t      ngx_current_msec;
అస్థిర ngx_సమయం_t     *ngx_cached_సమయం;
అస్థిర ngx_str_t       ngx_cached_err_log_సమయం;
అస్థిర ngx_str_t       ngx_cached_http_సమయం;
అస్థిర ngx_str_t       ngx_cached_http_log_సమయం;
అస్థిర ngx_str_t       ngx_cached_http_log_iso8601;
అస్థిర ngx_str_t       ngx_cached_syslog_సమయం;

#ఒకవేళ !(NGX_WIN32)

/*
 * localtime() and localtime_r() are not Async-Signal-Safe functions, therefore,
 * they must not be called by a signal handler, so we use the cached
 * GMT offset value. Fortunately the value is changed only two times a year.
 */

స్థిర ngx_int_t         cached_gmtoff;
#అయితే_ముగింపు

స్థిర ngx_సమయం_t        cached_సమయం[NGX_సమయం_SLOTS];
స్థిర u_char            cached_err_log_సమయం[NGX_సమయం_SLOTS]
                                    [పరిమాణం("1970/09/28 12:00:00")];
స్థిర u_char            cached_http_సమయం[NGX_సమయం_SLOTS]
                                    [పరిమాణం("Mon, 28 Sep 1970 06:00:00 GMT")];
స్థిర u_char            cached_http_log_సమయం[NGX_సమయం_SLOTS]
                                    [పరిమాణం("28/Sep/1970:12:00:00 +0600")];
స్థిర u_char            cached_http_log_iso8601[NGX_సమయం_SLOTS]
                                    [పరిమాణం("1970-09-28T12:00:00+06:00")];
స్థిర u_char            cached_syslog_సమయం[NGX_సమయం_SLOTS]
                                    [పరిమాణం("Sep 28 12:00:00")];


స్థిర అక్షరం  *week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
స్థిర అక్షరం  *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

శూన్య
ngx_సమయం_init(శూన్య)
{
    ngx_cached_err_log_సమయం.len = పరిమాణం("1970/09/28 12:00:00") - 1;
    ngx_cached_http_సమయం.len = పరిమాణం("Mon, 28 Sep 1970 06:00:00 GMT") - 1;
    ngx_cached_http_log_సమయం.len = పరిమాణం("28/Sep/1970:12:00:00 +0600") - 1;
    ngx_cached_http_log_iso8601.len = పరిమాణం("1970-09-28T12:00:00+06:00") - 1;
    ngx_cached_syslog_సమయం.len = పరిమాణం("Sep 28 12:00:00") - 1;

    ngx_cached_సమయం = &cached_సమయం[0];

    ngx_సమయం_update();
}


శూన్య
ngx_సమయం_update(శూన్య)
{
    u_char          *p0, *p1, *p2, *p3, *p4;
    ngx_tm_t         tm, gmt;
    సమయం_t           sec;
    ngx_uint_t       msec;
    ngx_సమయం_t      *tp;
    నిర్మాణం timeval   tv;

    ఒకవేళ (!ngx_trylock(&ngx_సమయం_తాళం)) {
        తిరిగి;
    }

    ngx_gettimeofday(&tv);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    ngx_current_msec = ngx_monotonic_సమయం(sec, msec);

    tp = &cached_సమయం[స్థానం];

    ఒకవేళ (tp->sec == sec) {
        tp->msec = msec;
        ngx_unlock(&ngx_సమయం_తాళం);
        తిరిగి;
    }

    ఒకవేళ (స్థానం == NGX_సమయం_SLOTS - 1) {
        స్థానం = 0;
    } లేకపోతే {
        స్థానం++;
    }

    tp = &cached_సమయం[స్థానం];

    tp->sec = sec;
    tp->msec = msec;

    ngx_gmtime(sec, &gmt);


    p0 = &cached_http_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p0, "%s, %02d %s %4d %02d:%02d:%02d GMT",
                       week[gmt.ngx_tm_wday], gmt.ngx_tm_mday,
                       months[gmt.ngx_tm_mon - 1], gmt.ngx_tm_year,
                       gmt.ngx_tm_hour, gmt.ngx_tm_min, gmt.ngx_tm_sec);

#ఒకవేళ (NGX_HAVE_GETTIMEZONE)

    tp->gmtoff = ngx_gettimezone();
    ngx_gmtime(sec + tp->gmtoff * 60, &tm);

#లేదావేళ (NGX_HAVE_GMTOFF)

    ngx_localtime(sec, &tm);
    cached_gmtoff = (ngx_int_t) (tm.ngx_tm_gmtoff / 60);
    tp->gmtoff = cached_gmtoff;

#లేకపోతే

    ngx_localtime(sec, &tm);
    cached_gmtoff = ngx_timezone(tm.ngx_tm_isdst);
    tp->gmtoff = cached_gmtoff;

#అయితే_ముగింపు


    p1 = &cached_err_log_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p1, "%4d/%02d/%02d %02d:%02d:%02d",
                       tm.ngx_tm_year, tm.ngx_tm_mon,
                       tm.ngx_tm_mday, tm.ngx_tm_hour,
                       tm.ngx_tm_min, tm.ngx_tm_sec);


    p2 = &cached_http_log_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p2, "%02d/%s/%d:%02d:%02d:%02d %c%02i%02i",
                       tm.ngx_tm_mday, months[tm.ngx_tm_mon - 1],
                       tm.ngx_tm_year, tm.ngx_tm_hour,
                       tm.ngx_tm_min, tm.ngx_tm_sec,
                       tp->gmtoff < 0 ? '-' : '+',
                       ngx_abs(tp->gmtoff / 60), ngx_abs(tp->gmtoff % 60));

    p3 = &cached_http_log_iso8601[స్థానం][0];

    (శూన్య) ngx_sprintf(p3, "%4d-%02d-%02dT%02d:%02d:%02d%c%02i:%02i",
                       tm.ngx_tm_year, tm.ngx_tm_mon,
                       tm.ngx_tm_mday, tm.ngx_tm_hour,
                       tm.ngx_tm_min, tm.ngx_tm_sec,
                       tp->gmtoff < 0 ? '-' : '+',
                       ngx_abs(tp->gmtoff / 60), ngx_abs(tp->gmtoff % 60));

    p4 = &cached_syslog_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p4, "%s %2d %02d:%02d:%02d",
                       months[tm.ngx_tm_mon - 1], tm.ngx_tm_mday,
                       tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ngx_జ్ఞాపకం_barrier();

    ngx_cached_సమయం = tp;
    ngx_cached_http_సమయం.దత్తాంశం = p0;
    ngx_cached_err_log_సమయం.దత్తాంశం = p1;
    ngx_cached_http_log_సమయం.దత్తాంశం = p2;
    ngx_cached_http_log_iso8601.దత్తాంశం = p3;
    ngx_cached_syslog_సమయం.దత్తాంశం = p4;

    ngx_unlock(&ngx_సమయం_తాళం);
}


స్థిర ngx_msec_t
ngx_monotonic_సమయం(సమయం_t sec, ngx_uint_t msec)
{
#ఒకవేళ (NGX_HAVE_CLOCK_MONOTONIC)
    నిర్మాణం timespec  ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    sec = ts.tv_sec;
    msec = ts.tv_nsec / 1000000;

#అయితే_ముగింపు

    తిరిగి (ngx_msec_t) sec * 1000 + msec;
}


#ఒకవేళ !(NGX_WIN32)

శూన్య
ngx_సమయం_sigsafe_update(శూన్య)
{
    u_char          *p, *p2;
    ngx_tm_t         tm;
    సమయం_t           sec;
    ngx_సమయం_t      *tp;
    నిర్మాణం timeval   tv;

    ఒకవేళ (!ngx_trylock(&ngx_సమయం_తాళం)) {
        తిరిగి;
    }

    ngx_gettimeofday(&tv);

    sec = tv.tv_sec;

    tp = &cached_సమయం[స్థానం];

    ఒకవేళ (tp->sec == sec) {
        ngx_unlock(&ngx_సమయం_తాళం);
        తిరిగి;
    }

    ఒకవేళ (స్థానం == NGX_సమయం_SLOTS - 1) {
        స్థానం = 0;
    } లేకపోతే {
        స్థానం++;
    }

    tp = &cached_సమయం[స్థానం];

    tp->sec = 0;

    ngx_gmtime(sec + cached_gmtoff * 60, &tm);

    p = &cached_err_log_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d",
                       tm.ngx_tm_year, tm.ngx_tm_mon,
                       tm.ngx_tm_mday, tm.ngx_tm_hour,
                       tm.ngx_tm_min, tm.ngx_tm_sec);

    p2 = &cached_syslog_సమయం[స్థానం][0];

    (శూన్య) ngx_sprintf(p2, "%s %2d %02d:%02d:%02d",
                       months[tm.ngx_tm_mon - 1], tm.ngx_tm_mday,
                       tm.ngx_tm_hour, tm.ngx_tm_min, tm.ngx_tm_sec);

    ngx_జ్ఞాపకం_barrier();

    ngx_cached_err_log_సమయం.దత్తాంశం = p;
    ngx_cached_syslog_సమయం.దత్తాంశం = p2;

    ngx_unlock(&ngx_సమయం_తాళం);
}

#అయితే_ముగింపు


u_char *
ngx_http_సమయం(u_char *buf, సమయం_t t)
{
    ngx_tm_t  tm;

    ngx_gmtime(t, &tm);

    తిరిగి ngx_sprintf(buf, "%s, %02d %s %4d %02d:%02d:%02d GMT",
                       week[tm.ngx_tm_wday],
                       tm.ngx_tm_mday,
                       months[tm.ngx_tm_mon - 1],
                       tm.ngx_tm_year,
                       tm.ngx_tm_hour,
                       tm.ngx_tm_min,
                       tm.ngx_tm_sec);
}


u_char *
ngx_http_cookie_సమయం(u_char *buf, సమయం_t t)
{
    ngx_tm_t  tm;

    ngx_gmtime(t, &tm);

    /*
     * Netscape 3.x does not understand 4-digit years at all and
     * 2-digit years more than "37"
     */

    తిరిగి ngx_sprintf(buf,
                       (tm.ngx_tm_year > 2037) ?
                                         "%s, %02d-%s-%d %02d:%02d:%02d GMT":
                                         "%s, %02d-%s-%02d %02d:%02d:%02d GMT",
                       week[tm.ngx_tm_wday],
                       tm.ngx_tm_mday,
                       months[tm.ngx_tm_mon - 1],
                       (tm.ngx_tm_year > 2037) ? tm.ngx_tm_year:
                                                 tm.ngx_tm_year % 100,
                       tm.ngx_tm_hour,
                       tm.ngx_tm_min,
                       tm.ngx_tm_sec);
}


శూన్య
ngx_gmtime(సమయం_t t, ngx_tm_t *tp)
{
    ngx_int_t   yday;
    ngx_uint_t  sec, min, hour, mday, mon, year, wday, days, leap;

    /* the calculation is valid for positive time_t only */

    ఒకవేళ (t < 0) {
        t = 0;
    }

    days = t / 86400;
    sec = t % 86400;

    /*
     * no more than 4 year digits supported,
     * truncate to December 31, 9999, 23:59:59
     */

    ఒకవేళ (days > 2932896) {
        days = 2932896;
        sec = 86399;
    }

    /* January 1, 1970 was Thursday */

    wday = (4 + days) % 7;

    hour = sec / 3600;
    sec %= 3600;
    min = sec / 60;
    sec %= 60;

    /*
     * the algorithm based on Gauss' formula,
     * see src/core/ngx_parse_time.c
     */

    /* days since March 1, 1 BC */
    days = days - (31 + 28) + 719527;

    /*
     * The "days" should be adjusted to 1 only, however, some March 1st's go
     * to previous year, so we adjust them to 2.  This causes also shift of the
     * last February days to next year, but we catch the case when "yday"
     * becomes negative.
     */

    year = (days + 2) * 400 / (365 * 400 + 100 - 4 + 1);

    yday = days - (365 * year + year / 4 - year / 100 + year / 400);

    ఒకవేళ (yday < 0) {
        leap = (year % 4 == 0) && (year % 100 || (year % 400 == 0));
        yday = 365 + leap + yday;
        year--;
    }

    /*
     * The empirical formula that maps "yday" to month.
     * There are at least 10 variants, some of them are:
     *     mon = (yday + 31) * 15 / 459
     *     mon = (yday + 31) * 17 / 520
     *     mon = (yday + 31) * 20 / 612
     */

    mon = (yday + 31) * 10 / 306;

    /* the Gauss' formula that evaluates days before the month */

    mday = yday - (367 * mon / 12 - 30) + 1;

    ఒకవేళ (yday >= 306) {

        year++;
        mon -= 10;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday -= 306;
         */

    } లేకపోతే {

        mon += 2;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday += 31 + 28 + leap;
         */
    }

    tp->ngx_tm_sec = (ngx_tm_sec_t) sec;
    tp->ngx_tm_min = (ngx_tm_min_t) min;
    tp->ngx_tm_hour = (ngx_tm_hour_t) hour;
    tp->ngx_tm_mday = (ngx_tm_mday_t) mday;
    tp->ngx_tm_mon = (ngx_tm_mon_t) mon;
    tp->ngx_tm_year = (ngx_tm_year_t) year;
    tp->ngx_tm_wday = (ngx_tm_wday_t) wday;
}


సమయం_t
ngx_తదుపరి_సమయం(సమయం_t when)
{
    సమయం_t     now, తదుపరి;
    నిర్మాణం tm  tm;

    now = ngx_సమయం();

    ngx_libc_localtime(now, &tm);

    tm.tm_hour = (పూర్ణాంకం) (when / 3600);
    when %= 3600;
    tm.tm_min = (పూర్ణాంకం) (when / 60);
    tm.tm_sec = (పూర్ణాంకం) (when % 60);

    తదుపరి = mktime(&tm);

    ఒకవేళ (తదుపరి == -1) {
        తిరిగి -1;
    }

    ఒకవేళ (తదుపరి - now > 0) {
        తిరిగి తదుపరి;
    }

    tm.tm_mday++;

    /* mktime() should normalize a date (Jan 32, etc) */

    తదుపరి = mktime(&tm);

    ఒకవేళ (తదుపరి != -1) {
        తిరిగి తదుపరి;
    }

    తిరిగి -1;
}
