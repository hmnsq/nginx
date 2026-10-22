
/*
 * Copyright (C) Nginx, Inc.
 */


#అన్తర్భావ <ngx_config.h>
#అన్తర్భావ <ngx_core.h>
#అన్తర్భావ <ngx_event.h>


#నిర్వచించు NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + పరిమాణం("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*ట్యాగ్*/ + 2 /* colon, space */


స్థిర అక్షరం *ngx_syslog_parse_వాదనలు(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
స్థిర ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
స్థిర శూన్య ngx_syslog_cleanup(శూన్య *దత్తాంశం);
స్థిర u_char *ngx_syslog_log_error(ngx_log_t *సంవర్గమానం, u_char *buf, కొలత_t len);


స్థిర అక్షరం  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    ఖాళీ
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
స్థిర అక్షరం  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", ఖాళీ
};

స్థిర ngx_log_t    ngx_syslog_dummy_log;
స్థిర ngx_event_t  ngx_syslog_dummy_event;


అక్షరం *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    ఒకవేళ (ngx_syslog_parse_వాదనలు(cf, peer) != NGX_CONF_OK) {
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (peer->server.sockaddr == ఖాళీ) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        తిరిగి NGX_CONF_ERROR;
    }

    ఒకవేళ (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    ఒకవేళ (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    ఒకవేళ (peer->ట్యాగ్.దత్తాంశం == ఖాళీ) {
        ngx_str_సమితి(&peer->ట్యాగ్, "nginx");
    }

    peer->hostname = &cf->చక్రం->hostname;
    peer->logp = &cf->చక్రం->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.పఠ = &ngx_syslog_dummy_event;
    peer->conn.లిఖ = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.సంవర్గమానం = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_కూడు(cf->pool, 0);
    ఒకవేళ (cln == ఖాళీ) {
        తిరిగి NGX_CONF_ERROR;
    }

    cln->దత్తాంశం = peer;
    cln->handler = ngx_syslog_cleanup;

    తిరిగి NGX_CONF_OK;
}


స్థిర అక్షరం *
ngx_syslog_parse_వాదనలు(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    కొలత_t       len;
    ngx_str_t   *విలువ;
    ngx_url_t    u;
    ngx_uint_t   i;

    విలువ = cf->వాదనలు->elts;

    p = విలువ[1].దత్తాంశం + పరిమాణం("syslog:") - 1;

    కొరకు ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        ఒకవేళ (comma != ఖాళీ) {
            len = comma - p;
            *comma = '\0';

        } లేకపోతే {
            len = విలువ[1].దత్తాంశం + విలువ[1].len - p;
        }

        ఒకవేళ (ngx_strncmp(p, "server=", 7) == 0) {

            ఒకవేళ (peer->server.sockaddr != ఖాళీ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                తిరిగి NGX_CONF_ERROR;
            }

            ngx_memzero(&u, పరిమాణం(ngx_url_t));

            u.url.దత్తాంశం = p + 7;
            u.url.len = len - 7;
            u.default_పోర్టు = 514;

            ఒకవేళ (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                ఒకవేళ (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                తిరిగి NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } లేకపోతే ఒకవేళ (ngx_strncmp(p, "facility=", 9) == 0) {

            ఒకవేళ (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                తిరిగి NGX_CONF_ERROR;
            }

            కొరకు (i = 0; facilities[i] != ఖాళీ; i++) {

                ఒకవేళ (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    వెళ్ళు తదుపరి;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            తిరిగి NGX_CONF_ERROR;

        } లేకపోతే ఒకవేళ (ngx_strncmp(p, "severity=", 9) == 0) {

            ఒకవేళ (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                తిరిగి NGX_CONF_ERROR;
            }

            కొరకు (i = 0; severities[i] != ఖాళీ; i++) {

                ఒకవేళ (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    వెళ్ళు తదుపరి;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            తిరిగి NGX_CONF_ERROR;

        } లేకపోతే ఒకవేళ (ngx_strncmp(p, "tag=", 4) == 0) {

            ఒకవేళ (peer->ట్యాగ్.దత్తాంశం != ఖాళీ) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                తిరిగి NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            ఒకవేళ (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                తిరిగి NGX_CONF_ERROR;
            }

            కొరకు (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                ఒకవేళ (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    తిరిగి NGX_CONF_ERROR;
                }
            }

            peer->ట్యాగ్.దత్తాంశం = p + 4;
            peer->ట్యాగ్.len = len - 4;

        } లేకపోతే ఒకవేళ (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } లేకపోతే {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            తిరిగి NGX_CONF_ERROR;
        }

    తదుపరి:

        ఒకవేళ (comma == ఖాళీ) {
            ఆపు;
        }

        p = comma + 1;
    }

    తిరిగి NGX_CONF_OK;
}


u_char *
ngx_syslog_కూడు_శీర్షిక(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    ఒకవేళ (peer->nohostname) {
        తిరిగి ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_సమయం,
                           &peer->ట్యాగ్);
    }

    తిరిగి ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_సమయం,
                       peer->hostname, &peer->ట్యాగ్);
}


శూన్య
ngx_syslog_writer(ngx_log_t *సంవర్గమానం, ngx_uint_t level, u_char *buf,
    కొలత_t len)
{
    u_char             *p, సందేశం[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          తల_len;
    ngx_syslog_peer_t  *peer;

    peer = సంవర్గమానం->wdata;

    ఒకవేళ (peer->busy) {
        తిరిగి;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_కూడు_శీర్షిక(peer, సందేశం);
    తల_len = p - సందేశం;

    len -= NGX_LINEFEED_కొలత;

    ఒకవేళ (len > NGX_SYSLOG_MAX_STR - తల_len) {
        len = NGX_SYSLOG_MAX_STR - తల_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (శూన్య) ngx_syslog_send(peer, సందేశం, p - సందేశం);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, కొలత_t len)
{
    ssize_t  n;

    ఒకవేళ (peer->సంవర్గమానం.handler == ఖాళీ) {
        peer->సంవర్గమానం = *peer->logp;
        peer->సంవర్గమానం.handler = ngx_syslog_log_error;
        peer->సంవర్గమానం.దత్తాంశం = peer;
        peer->సంవర్గమానం.action = "logging to syslog";
    }

    ఒకవేళ (peer->conn.fd == (ngx_socket_t) -1) {
        ఒకవేళ (ngx_syslog_init_peer(peer) != NGX_OK) {
            తిరిగి NGX_ERROR;
        }
    }

    ఒకవేళ (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } లేకపోతే {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    ఒకవేళ (n == NGX_ERROR) {

        ఒకవేళ (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    తిరిగి n;
}


స్థిర ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    ఒకవేళ (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                      ngx_socket_n " failed");
        తిరిగి NGX_ERROR;
    }

    ఒకవేళ (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        వెళ్ళు failed;
    }

    ఒకవేళ (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                      "connect() failed");
        వెళ్ళు failed;
    }

    peer->conn.fd = fd;
    peer->conn.సంవర్గమానం = &peer->సంవర్గమానం;

    /* UDP sockets are always ready to write */
    peer->conn.లిఖ->ready = 1;

    తిరిగి NGX_OK;

failed:

    ఒకవేళ (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    తిరిగి NGX_ERROR;
}


స్థిర శూన్య
ngx_syslog_cleanup(శూన్య *దత్తాంశం)
{
    ngx_syslog_peer_t  *peer = దత్తాంశం;

    /* prevents further use of this peer */
    peer->busy = 1;

    ఒకవేళ (peer->conn.fd == (ngx_socket_t) -1) {
        తిరిగి;
    }

    ఒకవేళ (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->సంవర్గమానం, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


స్థిర u_char *
ngx_syslog_log_error(ngx_log_t *సంవర్గమానం, u_char *buf, కొలత_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    ఒకవేళ (సంవర్గమానం->action) {
        p = ngx_snprintf(buf, len, " while %s", సంవర్గమానం->action);
        len -= p - buf;
    }

    peer = సంవర్గమానం->దత్తాంశం;

    ఒకవేళ (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    తిరిగి p;
}
