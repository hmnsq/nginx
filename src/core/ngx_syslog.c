
/*
 * Copyright (C) Nginx, Inc.
 */


#ضمن <ngx_config.h>
#ضمن <ngx_core.h>
#ضمن <ngx_event.h>


#تعریف_کرو NGX_SYSLOG_MAX_STR                                                    \
    NGX_MAX_ERROR_STR + حجم("<255>Jan 01 00:00:00 ") - 1                   \
    + (NGX_MAXHOSTNAMELEN - 1) + 1 /* space */                                \
    + 32 /*ٹیگ*/ + 2 /* colon, space */


ساكن حرف *ngx_syslog_parse_دلیلاں(ngx_conf_t *cf, ngx_syslog_peer_t *peer);
ساكن ngx_int_t ngx_syslog_init_peer(ngx_syslog_peer_t *peer);
ساكن فراغ ngx_syslog_cleanup(فراغ *انکڑے);
ساكن u_char *ngx_syslog_log_error(ngx_log_t *لوگارتھم, u_char *buf, ناپ_t len);


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

    إذا (ngx_syslog_parse_دلیلاں(cf, peer) != NGX_CONF_OK) {
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

    إذا (peer->ٹیگ.انکڑے == عدم) {
        ngx_str_سیٹ(&peer->ٹیگ, "nginx");
    }

    peer->hostname = &cf->چکر->hostname;
    peer->logp = &cf->چکر->new_log;

    peer->conn.fd = (ngx_socket_t) -1;

    peer->conn.اقرأ = &ngx_syslog_dummy_event;
    peer->conn.اكتب = &ngx_syslog_dummy_event;

    ngx_syslog_dummy_event.لوگارتھم = &ngx_syslog_dummy_log;

    cln = ngx_pool_cleanup_جوڑو(cf->pool, 0);
    إذا (cln == عدم) {
        أرجع NGX_CONF_ERROR;
    }

    cln->انکڑے = peer;
    cln->handler = ngx_syslog_cleanup;

    أرجع NGX_CONF_OK;
}


ساكن حرف *
ngx_syslog_parse_دلیلاں(ngx_conf_t *cf, ngx_syslog_peer_t *peer)
{
    u_char      *p, *comma, c;
    ناپ_t       len;
    ngx_str_t   *قدر;
    ngx_url_t    u;
    ngx_uint_t   i;

    قدر = cf->دلیلاں->elts;

    p = قدر[1].انکڑے + حجم("syslog:") - 1;

    لكل ( ;; ) {
        comma = (u_char *) ngx_strchr(p, ',');

        إذا (comma != عدم) {
            len = comma - p;
            *comma = '\0';

        } وإلا {
            len = قدر[1].انکڑے + قدر[1].len - p;
        }

        إذا (ngx_strncmp(p, "server=", 7) == 0) {

            إذا (peer->server.sockaddr != عدم) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "duplicate syslog \"server\"");
                أرجع NGX_CONF_ERROR;
            }

            ngx_memzero(&u, حجم(ngx_url_t));

            u.url.انکڑے = p + 7;
            u.url.len = len - 7;
            u.default_پورٹ = 514;

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
                    اذهب اگلا;
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
                    اذهب اگلا;
                }
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog severity \"%s\"", p + 9);
            أرجع NGX_CONF_ERROR;

        } وإلا إذا (ngx_strncmp(p, "tag=", 4) == 0) {

            إذا (peer->ٹیگ.انکڑے != عدم) {
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

            peer->ٹیگ.انکڑے = p + 4;
            peer->ٹیگ.len = len - 4;

        } وإلا إذا (len == 10 && ngx_strncmp(p, "nohostname", 10) == 0) {
            peer->nohostname = 1;

        } وإلا {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "unknown syslog parameter \"%s\"", p);
            أرجع NGX_CONF_ERROR;
        }

    اگلا:

        إذا (comma == عدم) {
            اكسر;
        }

        p = comma + 1;
    }

    أرجع NGX_CONF_OK;
}


u_char *
ngx_syslog_جوڑو_سرخی(ngx_syslog_peer_t *peer, u_char *buf)
{
    ngx_uint_t  pri;

    pri = peer->facility * 8 + peer->severity;

    إذا (peer->nohostname) {
        أرجع ngx_sprintf(buf, "<%ui>%V %V: ", pri, &ngx_cached_syslog_ویلا,
                           &peer->ٹیگ);
    }

    أرجع ngx_sprintf(buf, "<%ui>%V %V %V: ", pri, &ngx_cached_syslog_ویلا,
                       peer->hostname, &peer->ٹیگ);
}


فراغ
ngx_syslog_writer(ngx_log_t *لوگارتھم, ngx_uint_t level, u_char *buf,
    ناپ_t len)
{
    u_char             *p, پیغام[NGX_SYSLOG_MAX_STR];
    ngx_uint_t          سر_len;
    ngx_syslog_peer_t  *peer;

    peer = لوگارتھم->wdata;

    إذا (peer->busy) {
        أرجع;
    }

    peer->busy = 1;
    peer->severity = level - 1;

    p = ngx_syslog_جوڑو_سرخی(peer, پیغام);
    سر_len = p - پیغام;

    len -= NGX_LINEFEED_ناپ;

    إذا (len > NGX_SYSLOG_MAX_STR - سر_len) {
        len = NGX_SYSLOG_MAX_STR - سر_len;
    }

    p = ngx_snprintf(p, len, "%s", buf);

    (فراغ) ngx_syslog_send(peer, پیغام, p - پیغام);

    peer->busy = 0;
}


ssize_t
ngx_syslog_send(ngx_syslog_peer_t *peer, u_char *buf, ناپ_t len)
{
    ssize_t  n;

    إذا (peer->لوگارتھم.handler == عدم) {
        peer->لوگارتھم = *peer->logp;
        peer->لوگارتھم.handler = ngx_syslog_log_error;
        peer->لوگارتھم.انکڑے = peer;
        peer->لوگارتھم.action = "logging to syslog";
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
            ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
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
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_socket_n " failed");
        أرجع NGX_ERROR;
    }

    إذا (ngx_nonblocking(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_nonblocking_n " failed");
        اذهب failed;
    }

    إذا (connect(fd, peer->server.sockaddr, peer->server.socklen) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      "connect() failed");
        اذهب failed;
    }

    peer->conn.fd = fd;
    peer->conn.لوگارتھم = &peer->لوگارتھم;

    /* UDP sockets are always ready to write */
    peer->conn.اكتب->ready = 1;

    أرجع NGX_OK;

failed:

    إذا (ngx_close_socket(fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }

    أرجع NGX_ERROR;
}


ساكن فراغ
ngx_syslog_cleanup(فراغ *انکڑے)
{
    ngx_syslog_peer_t  *peer = انکڑے;

    /* prevents further use of this peer */
    peer->busy = 1;

    إذا (peer->conn.fd == (ngx_socket_t) -1) {
        أرجع;
    }

    إذا (ngx_close_socket(peer->conn.fd) == -1) {
        ngx_log_error(NGX_LOG_ALERT, &peer->لوگارتھم, ngx_socket_errno,
                      ngx_close_socket_n " failed");
    }
}


ساكن u_char *
ngx_syslog_log_error(ngx_log_t *لوگارتھم, u_char *buf, ناپ_t len)
{
    u_char             *p;
    ngx_syslog_peer_t  *peer;

    p = buf;

    إذا (لوگارتھم->action) {
        p = ngx_snprintf(buf, len, " while %s", لوگارتھم->action);
        len -= p - buf;
    }

    peer = لوگارتھم->انکڑے;

    إذا (peer) {
        p = ngx_snprintf(p, len, ", server: %V", &peer->server.name);
    }

    أرجع p;
}
