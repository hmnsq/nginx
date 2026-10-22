
/*
 * Copyright (C) Nginx, Inc.
 */


#شامل <ngx_config.h>
#شامل <ngx_core.h>
#شامل <ngx_event.h>


#تعریف_کرو NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + حجم("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*لیبل*/ + 2 /* colon, space */


ساکن حرف *ngx_syslog_parse_دلائل(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ساکن ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ساکن خالی ngx_syslog_cleanup(خالی *ڈیٹا);
ساکن u_char *ngx_syslog_log_error(ngx_log_t *لوگارتھم, u_char *buf, سائز_t len);


ساکن حرف  *facilities[] = {
    "kern", "user", "mail", "daemon", "auth", "intern", "lpr", "news", "uucp",
    "clock", "authpriv", "ftp", "ntp", "audit", "alert", "cron", "local0",
    "local1", "local2", "local3", "local4", "local5", "local6", "local7",
    تہی
};

/* note 'error/warn' like in nginx.conf, not 'err/warning' */
ساکن حرف  *severities[] = {
    "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug", تہی
};

ساکن ngx_log_t    ngx_syslog_dummy_log;
ساکن ngx_event_t  ngx_syslog_dummy_event;


حرف *
ngx_syslog_process_conf(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    ngx_pool_cleanup_t  *cln;

    peer->facility = NGX_CONF_UNSET_UINT;
    peer->severity = NGX_CONF_UNSET_UINT;

    اگر (ngx_syslog_parse_دلائل(cf, peer) != NGX_CONF_OK) {
        واپس NGX_CONF_ERROR;
    }

    اگر (peer->server.sockaddr == تہی) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "no syslog server specified");
        واپس NGX_CONF_ERROR;
    }

    اگر (peer->facility == NGX_CONF_UNSET_UINT) {
        peer->facility = 23; /* local7 */
    }

    اگر (peer->severity == NGX_CONF_UNSET_UINT) {
        peer->severity = 6; /* info */
    }

    اگر (peer->لیبل.ڈیٹا == تہی) {
        ngx_str_سیٹ(&peer->لیبل, "nginx");
    }

    peer->hostname = &cf->چکر->hostname;
    peer->logp = &cf->چکر->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.اقرأ = &ngx_syslog_dummy_event;
    peer->conn.اكتب = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.لوگارتھم = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
    اگر (cln == تہی) {
        واپس NGX_CONF_ERROR;
    }

    cln->ڈیٹا = peer;
    cln->handler = ngx_syslog_cleanup;

    واپس NGX_CONF_OK;
}


ساکن حرف *
ngx_syslog_parse_دلائل(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    سائز_t       len;
    ngx_str_t   *قدر;
    ngx_url_t    u;
    ngx_uint_t   i;

    قدر = cf->دلائل->elts;

    p = قدر[1].ڈیٹا + حجم("syslog:") - 1;

    برائے ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        اگر (comma != تہی) {
            len = comma - p;
            *comma = '\0';

        } ورنہ {
            len = قدر[1].ڈیٹا + قدر[1].len - p;
        }

        اگر (ngx_strncmp(p, "server=", 7) == 0) {

            اگر (peer->server.sockaddr != تہی) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                واپس NGX_CONF_ERROR;
            }

            ngx_memzero(&u, حجم(ngx_url_t));

            u.url.ڈیٹا = p + 7;
            u.url.len = len - 7;
            u.default_پورٹ = 514;

            اگر (ngx_parse_url(cf->pool, &u) != NGX_OK) {
                اگر (u.err) {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "%s in syslog server \"%V\"",
                                       u.err, &u.url);
                }

                واپس NGX_CONF_ERROR;
            }

            peer->server = u.addrs[0];

        } ورنہ اگر (ngx_strncmp(p, "facility=", 9) == 0) {

            اگر (peer->facility != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"facility\"");
                واپس NGX_CONF_ERROR;
            }

            برائے (i = 0; facilities[i] != تہی; i++) {

                اگر (ngx_strcmp(p + 9, facilities[i]) == 0) {
                    peer->facility = i;
                    جاؤ اگلا;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog facility \"%s\"", p + 9);
            واپس NGX_CONF_ERROR;

        } ورنہ اگر (ngx_strncmp(p, "severity=", 9) == 0) {

            اگر (peer->severity != NGX_CONF_UNSET_UINT) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"severity\"");
                واپس NGX_CONF_ERROR;
            }

            برائے (i = 0; severities[i] != تہی; i++) {

                اگر (ngx_strcmp(p + 9, severities[i]) == 0) {
                    peer->severity = i;
                    جاؤ اگلا;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            واپس NGX_CONF_ERROR;

        } ورنہ اگر (ngx_strncmp(p, "tag=", 4) == 0) {

            اگر (peer->لیبل.ڈیٹا != تہی) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"tag\"");
                واپس NGX_CONF_ERROR;
            }

            /*
             * RFC 3164: the TAG is a string of ABNF alphanumeric characters
             * that MUST NOT exceed 32 characters.
             */
            اگر (len - 4 > 32) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "syslog tag length exceeds 32");
                واپس NGX_CONF_ERROR;
            }

            برائے (i = 4; i < len; i++) {
                c = ngx_tolower(p[i]);

                اگر (c < '0' || (c > '9' && c < 'a' && c != '_') || c > 'z') {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "syslog \"tag\" only allows "
                                       "alphanumeric characters "
                                       "and underscore");
                    واپس NGX_CONF_ERROR;
                }
            }

            peer->لیبل.ڈیٹا = p + 4;
            peer->لیبل.len = len - 4;

        } ورنہ اگر (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } ورنہ {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            واپس NGX_CONF_ERROR;
        }

    اگلا:

        اگر (comma == تہی) {
            توڑو;
        }

        p = comma + 1;
    }

    واپس NGX_CONF_OK;
}


u_char *
ngx_syslog_جوڑو_سرخی(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    اگر (peer->nohostname) {
        واپس ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_وقت,
                           &peer->لیبل);
    }

    واپس ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_وقت,
                       peer->hostname, &peer->لیبل);
}


خالی
ngx_syslog_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    سائز_t len)
{
    u_char             *p, پیغام[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          سر_len;
    ngx_syslog_peer_t  *peer;

    peer = لوگارتھم->wdata;

    اگر (peer->busy) {
        واپس;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_جوڑو_سرخی(peer, پیغام);
    سر_len = p - پیغام;

    len -= NGX_LINEFEED_سائز;

    اگر (len > NGX_SYSLOG_MAX_STR - سر_len) {
        len = NGX_SYSLOG_MAX_STR - سر_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (خالی) ngx_syslog_send(peer, پیغام, p - پیغام);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, سائز_t len)
{
    ssize_t  n;

    اگر (peer->لوگارتھم.handler == تہی) {
        peer->لوگارتھم = *peer->logp;
        peer->لوگارتھم.handler = ngx_syslog_log_error;
        peer->لوگارتھم.ڈیٹا = peer;
        peer->لوگارتھم.action = "logging to syslog";
    }

    اگر (peer->conn.fd == (ngx_socket_t) -1) {
        اگر (ngx_syslog_init_peer(peer) != NGX_OK) {
            واپس NGX_ERROR;
        }
    }

    اگر (ngx_send) {
        n = ngx_send(&peer->conn, buf, len);

    } ورنہ {
        /* event module has not yet set ngx_io */
        n = ngx_os_io.send(&peer->conn, buf, len);
    }

    اگر (n == NGX_ERROR) {

        اگر (ngx_close_socket(peer->conn.fd) == -1) {
            ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                          ngx_close_socket_n " failed");
        }

        peer->conn.fd = (ngx_socket_t) -1;
    }

    واپس n;
}


ساکن ngx_int_t
ngx_syslog_init_peer(ngx_syslog_peer_t *peer)
{
    ngx_socket_t  fd;

    fd = ngx_socket(peer->server.sockaddr->sa_family, SOCK_DGRAM, 0);
    اگر (fd == (ngx_socket_t) -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        واپس NGX_ERROR;
    }

    اگر (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        جاؤ failed;
    }

    اگر (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      "connect() failed");
        جاؤ failed;
    }

    peer->conn.fd = fd;
    peer->conn.لوگارتھم = &peer->لوگارتھم;

    /* UDP sockets are always ready to write */
    peer->conn.اكتب->ready = 1;

    واپس NGX_OK;

failed:

    اگر (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    واپس NGX_ERROR;
}


ساکن خالی
ngx_syslog_cleanup(خالی *ڈیٹا)
{
    ngx_syslog_peer_t  *peer = ڈیٹا;

    /* prevents further use of this peer */
    peer->busy = 1;

    اگر (peer->conn.fd == (ngx_socket_t) -1) {
        واپس;
    }

    اگر (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ساکن u_char *
ngx_syslog_log_error(ngx_log_t *لوگارتھم, u_char *buf, سائز_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    اگر (لوگارتھم->action) {
        p = ngx_snprintf(buf, len, " while %s", لوگارتھم->action);
        len -= p - buf;
    }

    peer = لوگارتھم->ڈیٹا;

    اگر (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    واپس p;
}
