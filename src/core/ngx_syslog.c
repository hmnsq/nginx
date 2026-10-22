
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#وصف_ڪريو NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + حجم("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*ٽيگ*/ + 2 /* colon, space */


ساكن حرف *ngx_syslog_parse_دليلون(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ساكن ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ساكن فراغ ngx_syslog_cleanup(فراغ *معلومات);
ساكن u_char *ngx_syslog_log_error(ngx_log_t *لاگرٿم, u_char *buf, سائيز_t len);


ساكن حرف  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    عدم
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ساكن حرف  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", عدم
};

ساكن ngx_log_t    ngx_syslog_dummy_log;
ساكن ngx_event_t  ngx_syslog_dummy_event;


حرف *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    إذا (ngx_syslog_parse_دليلون(cf, peer) != NGX_CONF_OK) {
        أرجع NGX_CONF_ERROR;
    }

    إذا (peer->server.sockaddr == عدم) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        أرجع NGX_CONF_ERROR;
    }

    إذا (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    إذا (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    إذا (peer->ٽيگ.معلومات == عدم) {
        ngx_str_سيٽ(&peer->ٽيگ, "nginx");
    }

    peer->hostname = &cf->چڪر->hostname;
    peer->logp = &cf->چڪر->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.اقرأ = &ngx_syslog_dummy_event;
    peer->conn.اكتب = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.لاگرٿم = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_جوڙيو(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    cln->معلومات = peer;
    cln->handler = ngx_syslog_cleanup;

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_syslog_parse_دليلون(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    سائيز_t       len;
    ngx_str_t   *قدر;
    ngx_url_t    u;
    ngx_uint_t   i;

    قدر = cf->دليلون->elts;

    p = قدر[1].معلومات + حجم("syslog:") - 1;

    لكل ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        إذا (comma != عدم) {
            len = comma - p;
            *comma = '\0';

        } وإلا {
            len = قدر[1].معلومات + قدر[1].len - p;
        }

        إذا (ngx_strncmp(p, "server=", 7) == 0) {

            إذا (peer->server.sockaddr != عدم) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                أرجع NGX_CONF_ERROR;
            }

            ngx_memzero(&u, حجم(ngx_url_t));

            u.url.معلومات = p + 7;
            u.url.len = len - 7;
            u.default_پورٽ = 514;

            إذا (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                إذا (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                أرجع NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } وإلا إذا (ngx_strncmp(p, "facility=", 9) == 0) {

            إذا (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 0; facilities[i] != عدم; i++) {

                إذا (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    اذهب ايندڙ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            أرجع NGX_CONF_ERROR;

        } وإلا إذا (ngx_strncmp(p, "severity=", 9) == 0) {

            إذا (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 0; severities[i] != عدم; i++) {

                إذا (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    اذهب ايندڙ;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            أرجع NGX_CONF_ERROR;

        } وإلا إذا (ngx_strncmp(p, "tag=", 4) == 0) {

            إذا (peer->ٽيگ.معلومات != عدم) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                أرجع NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            إذا (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                أرجع NGX_CONF_ERROR;
            }

            لكل (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                إذا (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    أرجع NGX_CONF_ERROR;
                }
            }

            peer->ٽيگ.معلومات = p + 4;
            peer->ٽيگ.len = len - 4;

        } وإلا إذا (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } وإلا {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            أرجع NGX_CONF_ERROR;
        }

    ايندڙ:

        إذا (comma == عدم) {
            اكسر;
        }

        p = comma + 1;
    }

    أرجع NGX_CONF_OK;
}


u_char *
ngx_syslog_جوڙيو_سرو(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    إذا (peer->nohostname) {
        أرجع ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_وقت,
                           &peer->ٽيگ);
    }

    أرجع ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_وقت,
                       peer->hostname, &peer->ٽيگ);
}


فراغ
ngx_syslog_writer(ngx_log_t *لاگرٿم, ngx_uint_t level, u_char *buf,
    سائيز_t len)
{
    u_char             *p, نياپو[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          سر_len;
    ngx_syslog_peer_t  *peer;

    peer = لاگرٿم->wdata;

    إذا (peer->busy) {
        أرجع;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_جوڙيو_سرو(peer, نياپو);
    سر_len = p - نياپو;

    len -= NGX_LINEFEED_سائيز;

    إذا (len > NGX_SYSLOG_MAX_STR - سر_len) {
        len = NGX_SYSLOG_MAX_STR - سر_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (فراغ) ngx_syslog_send(peer, نياپو, p - نياپو);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, سائيز_t len)
{
    ssize_t  n;

    إذا (peer->لاگرٿم.handler == عدم) {
        peer->لاگرٿم = *peer->logp;
        peer->لاگرٿم.handler = ngx_syslog_log_error;
        peer->لاگرٿم.معلومات = peer;
        peer->لاگرٿم.action = "logging to syslog";
    }

    إذا (peer->conn.fd == (ngx_socket_t) -1) {
        إذا (ngx_syslog_init_peer(peer) != NGX_OK) {
            أرجع NGX_ERROR;
        }
    }

    إذا (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } وإلا {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    إذا (n == NGX_ERROR) {

        إذا (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    أرجع n;
}


ساكن ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    إذا (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        اذهب failed;
    }

    إذا (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                      "connect() failed");
        اذهب failed;
    }

    peer->conn.fd = fd;
    peer->conn.لاگرٿم = &peer->لاگرٿم;

    /* UDP sockets are always ready to write */
    peer->conn.اكتب->ready = 1;

    أرجع NGX_OK;

failed:

    إذا (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    أرجع NGX_ERROR;
}


ساكن فراغ
ngx_syslog_cleanup(فراغ *معلومات)
{
    ngx_syslog_peer_t  *peer = معلومات;

    /* prevents further use of this peer */
    peer->busy = 1;

    إذا (peer->conn.fd == (ngx_socket_t) -1) {
        أرجع;
    }

    إذا (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لاگرٿم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ساكن u_char *
ngx_syslog_log_error(ngx_log_t *لاگرٿم, u_char *buf, سائيز_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    إذا (لاگرٿم->action) {
        p = ngx_snprintf(buf, len, " while %s", لاگرٿم->action);
        len -= p - buf;
    }

    peer = لاگرٿم->معلومات;

    إذا (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    أرجع p;
}
